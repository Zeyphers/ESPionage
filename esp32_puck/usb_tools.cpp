// Guard: USBMode=default sets ARDUINO_USB_MODE=0 (TinyUSB).
// USBMode=hwcdc sets ARDUINO_USB_MODE=1 (hardware CDC, no TinyUSB).
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0

#include "usb_tools.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include "USBMSC.h"
#include <Preferences.h>
#include <esp_heap_caps.h>

// Forward-declare TinyUSB host-mounted check without pulling in tusb.h
extern "C" bool tud_mounted(void);

// --------------------------------------------------------------------------
// Hardware instances
// --------------------------------------------------------------------------
static USBHIDKeyboard Keyboard;
static USBHIDMouse    Mouse;
static USBMSC         MSC;

// --------------------------------------------------------------------------
// USB Mass Storage — 512 KB PSRAM-backed RAM disk
// --------------------------------------------------------------------------
#define MSC_SECTOR_SIZE  512
#define MSC_SECTOR_COUNT 1024   // 512 KB total
static uint8_t* _mscDisk = nullptr;
static bool     _mscMounted = false;

// Minimal FAT12 boot sector + FAT for a 512KB volume labelled "ESPIONAGE "
static void formatRamDisk() {
  if (!_mscDisk) return;
  memset(_mscDisk, 0, MSC_SECTOR_SIZE * MSC_SECTOR_COUNT);

  // ----- Boot sector (sector 0) -----
  uint8_t* bs = _mscDisk;
  bs[0]=0xEB; bs[1]=0x3C; bs[2]=0x90;        // jump + NOP
  memcpy(bs+3, "MSDOS5.0", 8);                // OEM name
  // BPB
  bs[11]=0x00; bs[12]=0x02;                   // bytes/sector = 512
  bs[13]=0x08;                                // sectors/cluster = 8 (4 KB clusters)
  bs[14]=0x01; bs[15]=0x00;                   // reserved sectors = 1
  bs[16]=0x02;                                // FAT copies = 2
  bs[17]=0x00; bs[18]=0x01;                   // root entries = 256 (16 sectors = matches data area at sector 19)
  bs[19]=0x00; bs[20]=0x04;                   // total sectors = 1024
  bs[21]=0xF8;                                // media type fixed disk
  bs[22]=0x01; bs[23]=0x00;                   // sectors/FAT = 1
  bs[24]=0x3F; bs[25]=0x00;                   // sectors/track
  bs[26]=0xFF; bs[27]=0x00;                   // heads
  bs[28]=0x00; bs[29]=0x00; bs[30]=0x00; bs[31]=0x00; // hidden sectors
  bs[32]=0x00; bs[33]=0x00; bs[34]=0x00; bs[35]=0x00; // large sectors
  bs[36]=0x80;                                // drive number
  bs[37]=0x00;                                // reserved
  bs[38]=0x29;                                // extended boot sig
  bs[39]=0x01; bs[40]=0x02; bs[41]=0x03; bs[42]=0x04; // volume ID
  memcpy(bs+43, "ESPIONAGE  ", 11);           // volume label (11 chars)
  memcpy(bs+54, "FAT12   ", 8);              // FS type
  bs[510]=0x55; bs[511]=0xAA;                // boot signature

  // ----- FAT1 (sector 1) -----
  uint8_t* fat = _mscDisk + MSC_SECTOR_SIZE;
  fat[0]=0xF8; fat[1]=0xFF; fat[2]=0xFF;    // media descriptor + EOC chain

  // ----- FAT2 (sector 2) -----
  uint8_t* fat2 = _mscDisk + 2*MSC_SECTOR_SIZE;
  fat2[0]=0xF8; fat2[1]=0xFF; fat2[2]=0xFF;

  // ----- Root directory (sectors 3..18, 512 entries × 32 bytes) -----
  // Volume label entry
  uint8_t* root = _mscDisk + 3*MSC_SECTOR_SIZE;
  memcpy(root, "ESPIONAGE  ", 11);
  root[11] = 0x08; // attribute: volume label

  // README.TXT directory entry (offset 32 from root start)
  uint8_t* de = root + 32;
  memcpy(de,    "README  TXT", 11);
  de[11] = 0x20;                            // normal file
  de[26] = 0x02; de[27] = 0x00;            // start cluster = 2
  const char* readme =
    "ESPionage USB Drive\r\n"
    "====================\r\n"
    "This drive is hosted in the ESP32-S3's PSRAM.\r\n"
    "Files are lost on reboot.\r\n";
  uint16_t rdLen = strlen(readme);
  de[28] = rdLen & 0xFF;
  de[29] = (rdLen >> 8) & 0xFF;
  de[30] = 0x00; de[31] = 0x00;

  // Write README content into cluster 2 (sector 19 = 3 reserved + 16 root)
  uint8_t* cl2 = _mscDisk + 19*MSC_SECTOR_SIZE;
  memcpy(cl2, readme, rdLen);

  // Mark cluster 2 as EOC in both FATs (FAT12 encoding)
  fat[3]=0xFF; fat[4]=0x0F;
  fat2[3]=0xFF; fat2[4]=0x0F;
}

// --------------------------------------------------------------------------
// FAT12 helpers
// --------------------------------------------------------------------------
// Geometry: 1 reserved + 2 FAT sectors + 16 root-dir sectors = sector 19 = first data sector
#define MSC_FIRST_DATA_SECTOR 19
#define MSC_SECTORS_PER_CLUSTER 8
#define MSC_CLUSTER_SIZE  (MSC_SECTOR_SIZE * MSC_SECTORS_PER_CLUSTER)
#define MSC_ROOT_ENTRIES  256
#define MSC_ROOT_SECTOR   3
#define MSC_MAX_CLUSTER   127  // clusters 2..126 → 125 usable clusters

static uint16_t getFat12(uint16_t cluster) {
  uint8_t* fat = _mscDisk + MSC_SECTOR_SIZE;
  uint32_t off = cluster + (cluster >> 1);
  uint16_t val = (uint16_t)fat[off] | ((uint16_t)fat[off+1] << 8);
  return (cluster & 1) ? (val >> 4) : (val & 0x0FFF);
}

static void setFat12(uint16_t cluster, uint16_t value) {
  uint32_t off = cluster + (cluster >> 1);
  for (int f = 1; f <= 2; f++) {
    uint8_t* fat = _mscDisk + f * MSC_SECTOR_SIZE;
    if (cluster & 1) {
      fat[off]   = (fat[off] & 0x0F) | (uint8_t)((value & 0x0F) << 4);
      fat[off+1] = (uint8_t)((value >> 4) & 0xFF);
    } else {
      fat[off]   = (uint8_t)(value & 0xFF);
      fat[off+1] = (fat[off+1] & 0xF0) | (uint8_t)((value >> 8) & 0x0F);
    }
  }
}

static uint16_t findFreeCluster(uint16_t start = 3) {
  for (uint16_t c = start; c < MSC_MAX_CLUSTER; c++)
    if (getFat12(c) == 0x000) return c;
  return 0;
}

static uint8_t* clusterPtr(uint16_t cluster) {
  uint32_t sector = MSC_FIRST_DATA_SECTOR + (uint32_t)(cluster - 2) * MSC_SECTORS_PER_CLUSTER;
  return _mscDisk + sector * MSC_SECTOR_SIZE;
}

static uint8_t* findFreeDirEntry() {
  uint8_t* root = _mscDisk + MSC_ROOT_SECTOR * MSC_SECTOR_SIZE;
  for (int i = 0; i < MSC_ROOT_ENTRIES; i++) {
    uint8_t* e = root + i * 32;
    if (e[0] == 0x00 || e[0] == 0xE5) return e;
  }
  return nullptr;
}

static uint8_t* findDirEntry(const char name83[11]) {
  uint8_t* root = _mscDisk + MSC_ROOT_SECTOR * MSC_SECTOR_SIZE;
  for (int i = 0; i < MSC_ROOT_ENTRIES; i++) {
    uint8_t* e = root + i * 32;
    if (e[0] == 0x00) break;
    if (e[0] == 0xE5 || (e[11] & 0x18)) continue;
    if (memcmp(e, name83, 11) == 0) return e;
  }
  return nullptr;
}

static void toFat83(const char* filename, char out[11]) {
  memset(out, ' ', 11);
  const char* dot = strrchr(filename, '.');
  int nameLen = dot ? (int)(dot - filename) : (int)strlen(filename);
  if (nameLen > 8) nameLen = 8;
  for (int i = 0; i < nameLen; i++) {
    char c = (char)toupper((unsigned char)filename[i]);
    if (strchr("\"*/:;<=>?[\\]|+,. ", c)) c = '_';
    out[i] = c;
  }
  if (dot && dot[1]) {
    int extLen = (int)strlen(dot + 1);
    if (extLen > 3) extLen = 3;
    for (int i = 0; i < extLen; i++) {
      char c = (char)toupper((unsigned char)dot[1+i]);
      if (strchr("\"*/:;<=>?[\\]|+,. ", c)) c = '_';
      out[8+i] = c;
    }
  }
}

// Deferred media-change flag — set from web task, applied from main loop via tick()
static volatile bool _pendingMediaChange = false;

// Upload state machine — one concurrent upload at a time
static struct {
  char     name83[11];
  uint16_t firstCluster;
  uint16_t curCluster;
  uint32_t curOffset;
  uint32_t totalLen;
  bool     active;
} _up;

bool UsbTools::uploadChunk(const char* filename, const uint8_t* data, size_t len, size_t index, bool final) {
  if (!_mscDisk || !_mscMounted) return false;

  if (index == 0) {
    toFat83(filename, _up.name83);
    // Remove existing file with same name
    uint8_t* ex = findDirEntry(_up.name83);
    if (ex) {
      uint16_t c = (uint16_t)ex[26] | ((uint16_t)ex[27] << 8);
      while (c >= 2 && c < 0xFF8) { uint16_t nx = getFat12(c); setFat12(c, 0); c = nx; }
      ex[0] = 0xE5;
    }
    uint16_t fc = findFreeCluster(3);
    if (!fc) return false;
    setFat12(fc, 0xFFF);
    _up.firstCluster = fc;
    _up.curCluster   = fc;
    _up.curOffset    = 0;
    _up.totalLen     = 0;
    _up.active       = true;
  }

  if (!_up.active) return false;

  size_t written = 0;
  while (written < len) {
    uint32_t space = MSC_CLUSTER_SIZE - _up.curOffset;
    size_t   chunk = (len - written) < (size_t)space ? (len - written) : (size_t)space;
    memcpy(clusterPtr(_up.curCluster) + _up.curOffset, data + written, chunk);
    _up.curOffset += chunk;
    written       += chunk;
    _up.totalLen  += chunk;

    if (_up.curOffset >= MSC_CLUSTER_SIZE && written < len) {
      uint16_t nx = findFreeCluster(_up.curCluster + 1);
      if (!nx) { _up.active = false; return false; }
      setFat12(_up.curCluster, nx);
      setFat12(nx, 0xFFF);
      _up.curCluster = nx;
      _up.curOffset  = 0;
    }
  }

  if (final) {
    uint8_t* e = findFreeDirEntry();
    if (!e) { _up.active = false; return false; }
    memset(e, 0, 32);
    memcpy(e, _up.name83, 11);
    e[11] = 0x20;
    e[26] = _up.firstCluster & 0xFF;
    e[27] = (_up.firstCluster >> 8) & 0xFF;
    e[28] = _up.totalLen & 0xFF;
    e[29] = (_up.totalLen >> 8) & 0xFF;
    e[30] = (_up.totalLen >> 16) & 0xFF;
    e[31] = (_up.totalLen >> 24) & 0xFF;
    _up.active = false;
    _pendingMediaChange = true;
  }
  return true;
}

void UsbTools::tick() {
  if (!_pendingMediaChange || !_mscMounted) return;
  _pendingMediaChange = false;
  MSC.mediaPresent(false);
  delay(400);
  MSC.mediaPresent(true);
}

uint8_t* UsbTools::getFileData(const char* filename, size_t& outSize) {
  if (!_mscDisk) { outSize = 0; return nullptr; }
  char name83[11];
  toFat83(filename, name83);
  uint8_t* e = findDirEntry(name83);
  if (!e) { outSize = 0; return nullptr; }
  outSize = (uint32_t)e[28] | ((uint32_t)e[29]<<8) | ((uint32_t)e[30]<<16) | ((uint32_t)e[31]<<24);
  if (!outSize) return (uint8_t*)malloc(1); // zero-length file
  uint8_t* buf = (uint8_t*)malloc(outSize);
  if (!buf) { outSize = 0; return nullptr; }
  uint16_t cluster = (uint16_t)e[26] | ((uint16_t)e[27] << 8);
  size_t remaining = outSize;
  size_t offset = 0;
  while (cluster >= 2 && cluster < 0xFF8 && remaining > 0) {
    size_t chunk = remaining < (size_t)MSC_CLUSTER_SIZE ? remaining : (size_t)MSC_CLUSTER_SIZE;
    memcpy(buf + offset, clusterPtr(cluster), chunk);
    offset    += chunk;
    remaining -= chunk;
    cluster    = getFat12(cluster);
  }
  return buf;
}

bool UsbTools::deleteFile(const char* filename) {
  if (!_mscDisk) return false;
  char name83[11];
  toFat83(filename, name83);
  uint8_t* e = findDirEntry(name83);
  if (!e) return false;
  uint16_t c = (uint16_t)e[26] | ((uint16_t)e[27] << 8);
  while (c >= 2 && c < 0xFF8) { uint16_t nx = getFat12(c); setFat12(c, 0); c = nx; }
  e[0] = 0xE5;
  _pendingMediaChange = true;
  return true;
}

String UsbTools::listFilesJson() {
  if (!_mscDisk) return "[]";
  String out = "[";
  bool first = true;
  uint8_t* root = _mscDisk + MSC_ROOT_SECTOR * MSC_SECTOR_SIZE;
  for (int i = 0; i < MSC_ROOT_ENTRIES; i++) {
    uint8_t* e = root + i * 32;
    if (e[0] == 0x00) break;
    if (e[0] == 0xE5 || (e[11] & 0x18)) continue;
    char name[13]; int ni = 0;
    for (int j = 0; j < 8 && e[j] != ' '; j++) name[ni++] = e[j];
    if (e[8] != ' ') { name[ni++] = '.'; for (int j = 8; j < 11 && e[j] != ' '; j++) name[ni++] = e[j]; }
    name[ni] = '\0';
    uint32_t sz = (uint32_t)e[28] | ((uint32_t)e[29]<<8) | ((uint32_t)e[30]<<16) | ((uint32_t)e[31]<<24);
    if (!first) out += ',';
    out += "{\"name\":\"" + String(name) + "\",\"size\":" + String(sz) + "}";
    first = false;
  }
  return out + "]";
}

static int32_t mscRead(uint32_t lba, uint32_t offset, void* buf, uint32_t sz) {
  if (!_mscDisk) return -1;
  uint32_t base = lba * MSC_SECTOR_SIZE + offset;
  if (base + sz > (uint32_t)(MSC_SECTOR_SIZE * MSC_SECTOR_COUNT)) return -1;
  memcpy(buf, _mscDisk + base, sz);
  return sz;
}

static int32_t mscWrite(uint32_t lba, uint32_t offset, uint8_t* buf, uint32_t sz) {
  if (!_mscDisk) return -1;
  uint32_t base = lba * MSC_SECTOR_SIZE + offset;
  if (base + sz > (uint32_t)(MSC_SECTOR_SIZE * MSC_SECTOR_COUNT)) return -1;
  memcpy(_mscDisk + base, buf, sz);
  return sz;
}

// RAM disk — no flush needed

// --------------------------------------------------------------------------
// State
// --------------------------------------------------------------------------
static volatile bool _jiggling   = false;
static volatile bool _hidRunning = false;
static int           _jiggleIntervalMs = 30000;

static TaskHandle_t _jiggleTask = nullptr;
static TaskHandle_t _duckyTask  = nullptr;

// Shared payload buffer — written from web-server task, read from ducky task.
// Max Ducky script size: 2 KB is plenty for any realistic payload.
static char   _duckyPayload[2048];
static bool   _duckyLoop = false;

// --------------------------------------------------------------------------
// Key-name → key-code table
// --------------------------------------------------------------------------
struct KeyEntry { const char* name; uint8_t code; };
static const KeyEntry KEY_MAP[] = {
  { "ENTER",      KEY_RETURN      },
  { "RETURN",     KEY_RETURN      },
  { "BACKSPACE",  KEY_BACKSPACE   },
  { "TAB",        KEY_TAB         },
  { "ESCAPE",     KEY_ESC         },
  { "ESC",        KEY_ESC         },
  { "DELETE",     KEY_DELETE      },
  { "INSERT",     KEY_INSERT      },
  { "HOME",       KEY_HOME        },
  { "END",        KEY_END         },
  { "PAGEUP",     KEY_PAGE_UP     },
  { "PAGEDOWN",   KEY_PAGE_DOWN   },
  { "UP",         KEY_UP_ARROW    },
  { "DOWN",       KEY_DOWN_ARROW  },
  { "LEFT",       KEY_LEFT_ARROW  },
  { "RIGHT",      KEY_RIGHT_ARROW },
  { "SPACE",      KEY_SPACE       },
  { "CAPS_LOCK",  KEY_CAPS_LOCK   },
  { "PRINTSCREEN",KEY_PRINT_SCREEN},
  { "F1",  KEY_F1  }, { "F2",  KEY_F2  }, { "F3",  KEY_F3  },
  { "F4",  KEY_F4  }, { "F5",  KEY_F5  }, { "F6",  KEY_F6  },
  { "F7",  KEY_F7  }, { "F8",  KEY_F8  }, { "F9",  KEY_F9  },
  { "F10", KEY_F10 }, { "F11", KEY_F11 }, { "F12", KEY_F12 },
  // Modifier keys (held with combo lines)
  { "CTRL",       KEY_LEFT_CTRL   },
  { "CONTROL",    KEY_LEFT_CTRL   },
  { "SHIFT",      KEY_LEFT_SHIFT  },
  { "ALT",        KEY_LEFT_ALT    },
  { "GUI",        KEY_LEFT_GUI    }, // Windows key / Cmd
  { "WIN",        KEY_LEFT_GUI    },
  { "COMMAND",    KEY_LEFT_GUI    },
  { "META",       KEY_LEFT_GUI    },
  { "RCTRL",      KEY_RIGHT_CTRL  },
  { "RSHIFT",     KEY_RIGHT_SHIFT },
  { "RALT",       KEY_RIGHT_ALT   },
  { nullptr, 0 }
};

static uint8_t resolveKey(const char* token, int len) {
  // Check named keys (case-insensitive)
  for (int i = 0; KEY_MAP[i].name != nullptr; i++) {
    if (strncasecmp(token, KEY_MAP[i].name, len) == 0 &&
        KEY_MAP[i].name[len] == '\0') {
      return KEY_MAP[i].code;
    }
  }
  // Single printable character
  if (len == 1) return (uint8_t)token[0];
  return 0;
}

// --------------------------------------------------------------------------
// Ducky script interpreter (blocking — runs in its own FreeRTOS task)
// --------------------------------------------------------------------------
static void runDuckyScript(const char* script) {
  const char* p = script;
  while (*p && _hidRunning) {
    // Find end of line
    const char* eol = p;
    while (*eol && *eol != '\n' && *eol != '\r') eol++;
    int lineLen = eol - p;

    // Skip blank lines and carriage returns
    if (lineLen == 0) { p = eol + 1; continue; }

    // Copy line for manipulation
    char line[512];
    int copyLen = lineLen < 511 ? lineLen : 511;
    memcpy(line, p, copyLen);
    line[copyLen] = '\0';

    // Trim trailing whitespace
    int tl = strlen(line);
    while (tl > 0 && (line[tl-1] == ' ' || line[tl-1] == '\t')) line[--tl] = '\0';

    // --- REM: comment
    if (strncasecmp(line, "REM", 3) == 0 && (line[3] == ' ' || line[3] == '\0')) {
      // skip

    // --- STRING: type text (break after each char so stop() can interrupt)
    } else if (strncasecmp(line, "STRING ", 7) == 0) {
      for (const char* c = line + 7; *c && _hidRunning; c++) {
        Keyboard.write((uint8_t)*c);
      }
      delay(10);

    // --- STRINGLN: type text + enter
    } else if (strncasecmp(line, "STRINGLN ", 9) == 0) {
      for (const char* c = line + 9; *c && _hidRunning; c++) {
        Keyboard.write((uint8_t)*c);
      }
      if (_hidRunning) Keyboard.write('\n');
      delay(10);

    // --- DELAY: wait N milliseconds (broken into chunks so stop() is snappy)
    } else if (strncasecmp(line, "DELAY ", 6) == 0) {
      int ms = atoi(line + 6);
      if (ms > 0 && ms <= 60000) {
        int waited = 0;
        while (_hidRunning && waited < ms) {
          int step = (ms - waited) > 50 ? 50 : (ms - waited);
          delay(step);
          waited += step;
        }
      }

    // --- REPEAT: not implemented (rare in real payloads)
    } else if (strncasecmp(line, "REPEAT", 6) == 0) {
      // skip

    // --- Key combo OR literal text
    // First pass: check if every whitespace-separated token resolves to a key.
    // If yes → press them together as a combo (e.g. "GUI r", "CTRL ALT DELETE").
    // If no  → treat the whole line as literal text to type (user convenience —
    //           lets people just type "hello world" without a STRING prefix).
    } else {
      // Copy line again for tokenization (strtok mutates)
      char scratch[512];
      strncpy(scratch, line, sizeof(scratch) - 1);
      scratch[sizeof(scratch) - 1] = '\0';

      bool allResolve = true;
      int  tokenCount = 0;
      char* tok = strtok(scratch, " \t");
      while (tok) {
        tokenCount++;
        if (!resolveKey(tok, strlen(tok))) { allResolve = false; break; }
        tok = strtok(nullptr, " \t");
      }

      if (tokenCount > 0 && allResolve) {
        // Real key combo — press all, release all
        Keyboard.releaseAll();
        strncpy(scratch, line, sizeof(scratch) - 1);
        scratch[sizeof(scratch) - 1] = '\0';
        tok = strtok(scratch, " \t");
        while (tok) {
          uint8_t code = resolveKey(tok, strlen(tok));
          if (code) Keyboard.press(code);
          tok = strtok(nullptr, " \t");
        }
        delay(60);
        Keyboard.releaseAll();
        delay(60);
      } else {
        // Fallback: type the line as literal text (interruptible)
        for (const char* c = line; *c && _hidRunning; c++) {
          Keyboard.write((uint8_t)*c);
        }
        delay(10);
      }
    }

    // Advance past EOL (handle \r\n)
    p = eol;
    if (*p == '\r') p++;
    if (*p == '\n') p++;
  }
  Keyboard.releaseAll();
}

static void duckyTaskFn(void* /*param*/) {
  // Wait up to 3s for USB host to enumerate us. Without this, Keyboard.print
  // silently no-ops if the script fires before the OS has mounted the HID.
  int waited = 0;
  while (!tud_mounted() && _hidRunning && waited < 3000) {
    delay(100);
    waited += 100;
  }
  // Small extra settle delay so the host's HID stack is ready to receive
  delay(250);

  do {
    runDuckyScript(_duckyPayload);
    // Brief pause between loops, interruptible
    int waited = 0;
    while (_duckyLoop && _hidRunning && waited < 300) {
      delay(50);
      waited += 50;
    }
  } while (_duckyLoop && _hidRunning);

  _hidRunning = false;
  _duckyTask  = nullptr;
  vTaskDelete(nullptr);
}

// --------------------------------------------------------------------------
// Mouse jiggler task
// --------------------------------------------------------------------------
static void jiggleTaskFn(void* /*param*/) {
  // Initial tiny move immediately so the user sees it worked
  Mouse.move(2, 0); delay(30); Mouse.move(-2, 0);

  while (_jiggling) {
    // Sleep in small chunks so we can be killed quickly
    int waited = 0;
    while (_jiggling && waited < _jiggleIntervalMs) {
      delay(250);
      waited += 250;
    }
    if (_jiggling) {
      Mouse.move(3, 0); delay(30); Mouse.move(-3, 0);
    }
  }
  _jiggleTask = nullptr;
  vTaskDelete(nullptr);
}

// --------------------------------------------------------------------------
// VID/PID spoof — saved to NVS, applied on next USB enumeration (boot)
// --------------------------------------------------------------------------
static void applySpoofPrefs() {
  Preferences prefs;
  prefs.begin("usb_spoof", true); // read-only
  uint16_t vid = prefs.getUShort("vid", 0x05AC);
  uint16_t pid = prefs.getUShort("pid", 0x0267);
  String   pn  = prefs.getString("product",  "Apple Wireless Keyboard");
  String   mfr = prefs.getString("mfr",      "Apple Inc.");
  prefs.end();
  USB.VID(vid);
  USB.PID(pid);
  USB.productName(pn.c_str());
  USB.manufacturerName(mfr.c_str());
  Serial.printf("[USB] VID=%04X PID=%04X \"%s\"\n", vid, pid, pn.c_str());
}

static void saveSpoofPrefs(uint16_t vid, uint16_t pid,
                            const char* product, const char* mfr) {
  Preferences prefs;
  prefs.begin("usb_spoof", false);
  prefs.putUShort("vid", vid);
  prefs.putUShort("pid", pid);
  prefs.putString("product", product);
  prefs.putString("mfr",     mfr);
  prefs.end();
}

// --------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------
void UsbTools::begin() {
  // Apply saved VID/PID spoof (defaults to Apple Wireless Keyboard A1314).
  // The Apple spoof prevents macOS from showing the "Keyboard Setup Assistant" dialog.
  applySpoofPrefs();

  // Register keyboard + mouse BEFORE USB.begin() so their descriptors are
  // included in the device's composite descriptor during enumeration.
  // Register keyboard + mouse + MSC BEFORE USB.begin()
  Keyboard.begin();
  Mouse.begin();

  // Always allocate the RAM disk so the web file manager works regardless of USB MSC state
  _mscDisk = (uint8_t*)heap_caps_malloc(
    MSC_SECTOR_SIZE * MSC_SECTOR_COUNT, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!_mscDisk)
    _mscDisk = (uint8_t*)malloc(MSC_SECTOR_SIZE * 256);
  if (_mscDisk) {
    formatRamDisk();
    Serial.printf("[USB MSC] RAM disk allocated: %u KB\n", MSC_SECTOR_COUNT / 2);
  }

  // Register MSC with USB only if explicitly requested (persisted in NVS)
  {
    Preferences prefs;
    prefs.begin("usb_spoof", true);
    bool msc = prefs.getBool("msc", false);
    prefs.end();
    if (msc && _mscDisk) {
      MSC.vendorID("ESPIO");
      MSC.productID("RamDisk");
      MSC.productRevision("1.0");
      MSC.onRead(mscRead);
      MSC.onWrite(mscWrite);
      MSC.mediaPresent(true);
      MSC.begin(MSC_SECTOR_COUNT, MSC_SECTOR_SIZE);
      _mscMounted = true;
    }
  }

  USB.begin();
}

void UsbTools::handleAction(const char* type, const char* payload,
                             int interval, bool looping,
                             const char* /*os*/,
                             const char* vid, const char* pid) {
  if (strcmp(type, "stop") == 0) {
    stop();
    return;
  }

  if (strcmp(type, "mouse") == 0) {
    if (_jiggleTask) {
      _jiggling = false;
      vTaskDelay(pdMS_TO_TICKS(300));
      if (_jiggleTask) { vTaskDelete(_jiggleTask); _jiggleTask = nullptr; }
    }
    _jiggleIntervalMs = max(5, min(300, interval)) * 1000;
    _jiggling = true;
    xTaskCreate(jiggleTaskFn, "jiggle", 2048, nullptr, 1, &_jiggleTask);
    return;
  }

  if (strcmp(type, "hid") == 0) {
    if (_duckyTask) {
      _hidRunning = false;
      vTaskDelay(pdMS_TO_TICKS(300));
      if (_duckyTask) { vTaskDelete(_duckyTask); _duckyTask = nullptr; }
    }
    Keyboard.releaseAll();
    strncpy(_duckyPayload, payload ? payload : "", sizeof(_duckyPayload) - 1);
    _duckyPayload[sizeof(_duckyPayload) - 1] = '\0';
    _duckyLoop   = looping;
    _hidRunning  = true;
    xTaskCreate(duckyTaskFn, "ducky", 4096, nullptr, 1, &_duckyTask);
    return;
  }

  if (strcmp(type, "storage") == 0) {
    // Save MSC flag, then reboot — MSC is registered before USB.begin() at boot
    Preferences prefs;
    prefs.begin("usb_spoof", false);
    prefs.putBool("msc", true);
    prefs.end();
    Serial.println("[USB MSC] rebooting to mount RAM disk...");
    vTaskDelay(pdMS_TO_TICKS(300));
    esp_restart();
    return;
  }

  if (strcmp(type, "spoof") == 0) {
    // Parse VID/PID hex strings, save to NVS, then reboot
    uint16_t newVid = (uint16_t)strtol(vid && vid[0] ? vid : "05AC", nullptr, 16);
    uint16_t newPid = (uint16_t)strtol(pid && pid[0] ? pid : "0267", nullptr, 16);

    // Pick a plausible product/manufacturer name based on VID
    const char* product = "HID Device";
    const char* mfr     = "Generic";
    if (newVid == 0x05AC) { product = "Apple Wireless Keyboard"; mfr = "Apple Inc."; }
    else if (newVid == 0x1050) { product = "YubiKey OTP+FIDO"; mfr = "Yubico"; }
    else if (newVid == 0x045E) { product = "USB Keyboard"; mfr = "Microsoft"; }
    else if (newVid == 0x046D) { product = "USB Receiver"; mfr = "Logitech"; }
    else if (newVid == 0x0781) { product = "USB Flash Drive"; mfr = "SanDisk"; }
    else if (newVid == 0x05AC && newPid == 0x12A8) { product = "iPhone"; mfr = "Apple Inc."; }
    else if (newVid == 0x18D1) { product = "Android"; mfr = "Google"; }
    else if (newVid == 0x1B1C) { product = "USB Gaming Keyboard"; mfr = "Corsair"; }

    saveSpoofPrefs(newVid, newPid, product, mfr);
    Serial.printf("[USB] spoof saved VID=%04X PID=%04X — rebooting\n", newVid, newPid);
    vTaskDelay(pdMS_TO_TICKS(300));
    esp_restart();
    return;
  }
}

void UsbTools::stop() {
  _jiggling   = false;
  _hidRunning = false;
  _duckyLoop  = false;
  vTaskDelay(pdMS_TO_TICKS(350));
  if (_jiggleTask) { vTaskDelete(_jiggleTask); _jiggleTask = nullptr; }
  if (_duckyTask)  { vTaskDelete(_duckyTask);  _duckyTask  = nullptr; }
  Keyboard.releaseAll();

  // If MSC was mounted, clear the flag so next reboot goes back to HID-only
  if (_mscMounted) {
    Preferences prefs;
    prefs.begin("usb_spoof", false);
    prefs.putBool("msc", false);
    prefs.end();
    _mscMounted = false;
  }
}

bool UsbTools::isMounted() {
  return tud_mounted();
}

bool UsbTools::isActive() {
  return _jiggling || _hidRunning || _mscMounted;
}

String UsbTools::getStatusJson() {
  Preferences prefs;
  prefs.begin("usb_spoof", true);
  uint16_t vid = prefs.getUShort("vid", 0x05AC);
  uint16_t pid = prefs.getUShort("pid", 0x0267);
  String pn    = prefs.getString("product", "Apple Wireless Keyboard");
  bool msc     = prefs.getBool("msc", false);
  prefs.end();

  char buf[16];
  String s = "{";
  s += "\"mounted\":"    + String(tud_mounted()  ? "true" : "false") + ",";
  s += "\"jiggling\":"   + String(_jiggling      ? "true" : "false") + ",";
  s += "\"hidRunning\":" + String(_hidRunning    ? "true" : "false") + ",";
  s += "\"mscActive\":"  + String(_mscMounted    ? "true" : "false") + ",";
  snprintf(buf, sizeof(buf), "%04X", vid); s += "\"vid\":\"" + String(buf) + "\",";
  snprintf(buf, sizeof(buf), "%04X", pid); s += "\"pid\":\"" + String(buf) + "\",";
  s += "\"product\":\"" + pn + "\"";
  s += "}";
  return s;
}

#endif // ARDUINO_USB_MODE == 0 (TinyUSB / USBMode=default)
