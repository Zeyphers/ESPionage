// BLE HID keyboard + mouse for ESPionage.
// Uses NimBLE-Arduino 2.x's NimBLEHIDDevice to present a composite
// keyboard + mouse to whatever phone or laptop pairs with it.
// Shares the NimBLE stack with ble_tools — call stop() on scanning
// before activating this module.

#include "ble_hid.h"
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#include <NimBLEServer.h>
#include <NimBLEAdvertising.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace BleHid {

// --------------------------------------------------------------------------
// HID report descriptor — composite keyboard (ID 1) + mouse (ID 2)
// --------------------------------------------------------------------------
static const uint8_t hidReportMap[] = {
  // ---- Keyboard (Report ID 1) ----
  0x05,0x01, 0x09,0x06, 0xA1,0x01,
  0x85,0x01,                         // Report ID 1
  0x05,0x07,
  0x19,0xE0, 0x29,0xE7,              // modifier keys (8 bits)
  0x15,0x00, 0x25,0x01,
  0x75,0x01, 0x95,0x08, 0x81,0x02,
  0x75,0x08, 0x95,0x01, 0x81,0x01,   // reserved byte
  0x05,0x08, 0x19,0x01, 0x29,0x05,   // LED output (5 bits)
  0x91,0x02, 0x75,0x03, 0x95,0x01, 0x91,0x01,
  0x75,0x08, 0x95,0x06,              // key array (6 keys)
  0x15,0x00, 0x25,0x73,
  0x05,0x07, 0x19,0x00, 0x29,0x73,
  0x81,0x00,
  0xC0,
  // ---- Mouse (Report ID 2) ----
  0x05,0x01, 0x09,0x02, 0xA1,0x01,
  0x85,0x02,                         // Report ID 2
  0x09,0x01, 0xA1,0x00,
  0x05,0x09, 0x19,0x01, 0x29,0x03,   // 3 buttons
  0x15,0x00, 0x25,0x01,
  0x95,0x03, 0x75,0x01, 0x81,0x02,
  0x95,0x01, 0x75,0x05, 0x81,0x01,   // 5-bit padding
  0x05,0x01, 0x09,0x30, 0x09,0x31, 0x09,0x38, // X, Y, Wheel
  0x15,0x81, 0x25,0x7F,
  0x75,0x08, 0x95,0x03, 0x81,0x06,
  0xC0, 0xC0
};

// --------------------------------------------------------------------------
// ASCII → HID keycode helper
// --------------------------------------------------------------------------
struct KeyPress { uint8_t mod; uint8_t code; };

static KeyPress asciiToHid(char c) {
  if (c >= 'a' && c <= 'z') return {0x00, (uint8_t)(0x04 + c - 'a')};
  if (c >= 'A' && c <= 'Z') return {0x02, (uint8_t)(0x04 + c - 'A')};
  if (c >= '1' && c <= '9') return {0x00, (uint8_t)(0x1E + c - '1')};
  switch (c) {
    case '0': return {0x00,0x27}; case '\n': case '\r': return {0x00,0x28};
    case '\t': return {0x00,0x2B}; case ' ':  return {0x00,0x2C};
    case '-':  return {0x00,0x2D}; case '=':  return {0x00,0x2E};
    case '[':  return {0x00,0x2F}; case ']':  return {0x00,0x30};
    case '\\': return {0x00,0x31}; case ';':  return {0x00,0x33};
    case '\'': return {0x00,0x34}; case '`':  return {0x00,0x35};
    case ',':  return {0x00,0x36}; case '.':  return {0x00,0x37};
    case '/':  return {0x00,0x38};
    // Shifted
    case '!': return {0x02,0x1E}; case '@': return {0x02,0x1F};
    case '#': return {0x02,0x20}; case '$': return {0x02,0x21};
    case '%': return {0x02,0x22}; case '^': return {0x02,0x23};
    case '&': return {0x02,0x24}; case '*': return {0x02,0x25};
    case '(': return {0x02,0x26}; case ')': return {0x02,0x27};
    case '_': return {0x02,0x2D}; case '+': return {0x02,0x2E};
    case '{': return {0x02,0x2F}; case '}': return {0x02,0x30};
    case '|': return {0x02,0x31}; case ':': return {0x02,0x33};
    case '"': return {0x02,0x34}; case '~': return {0x02,0x35};
    case '<': return {0x02,0x36}; case '>': return {0x02,0x37};
    case '?': return {0x02,0x38};
    default:  return {0x00,0x00};
  }
}

// Named-key → {modifier, keycode}
struct NamedKey { const char* name; uint8_t mod; uint8_t code; };
static const NamedKey NAMED_KEYS[] = {
  {"ENTER",      0x00,0x28}, {"RETURN",     0x00,0x28},
  {"BACKSPACE",  0x00,0x2A}, {"TAB",        0x00,0x2B},
  {"ESC",        0x00,0x29}, {"ESCAPE",     0x00,0x29},
  {"DELETE",     0x00,0x4C}, {"INSERT",     0x00,0x49},
  {"HOME",       0x00,0x4A}, {"END",        0x00,0x4D},
  {"PAGEUP",     0x00,0x4B}, {"PAGEDOWN",   0x00,0x4E},
  {"UP",         0x00,0x52}, {"DOWN",       0x00,0x51},
  {"LEFT",       0x00,0x50}, {"RIGHT",      0x00,0x4F},
  {"SPACE",      0x00,0x2C}, {"CAPS_LOCK",  0x00,0x39},
  {"F1",0x00,0x3A},{"F2",0x00,0x3B},{"F3",0x00,0x3C},{"F4",0x00,0x3D},
  {"F5",0x00,0x3E},{"F6",0x00,0x3F},{"F7",0x00,0x40},{"F8",0x00,0x41},
  {"F9",0x00,0x42},{"F10",0x00,0x43},{"F11",0x00,0x44},{"F12",0x00,0x45},
  {"CTRL",    0x01,0x00},{"CONTROL", 0x01,0x00},
  {"SHIFT",   0x02,0x00},
  {"ALT",     0x04,0x00},
  {"GUI",     0x08,0x00},{"WIN",     0x08,0x00},
  {"COMMAND", 0x08,0x00},{"META",    0x08,0x00},
  {"RCTRL",   0x10,0x00},{"RSHIFT",  0x20,0x00},
  {"RALT",    0x40,0x00},
  {nullptr,0,0}
};

// --------------------------------------------------------------------------
// State
// --------------------------------------------------------------------------
static NimBLEServer*    _server    = nullptr;
static NimBLEHIDDevice* _hid       = nullptr;
static NimBLECharacteristic* _kbInput    = nullptr;
static NimBLECharacteristic* _mouseInput = nullptr;

static volatile bool _advertising = false;
static volatile bool _connected   = false;
static volatile bool _jiggling    = false;
static volatile bool _hidRunning  = false;
static volatile bool _duckyLoop   = false;
static int           _jiggleInterval = 30000; // ms
static char          _devName[64]  = "ESPionage KB";

static TaskHandle_t _jiggleTask = nullptr;
static TaskHandle_t _duckyTask  = nullptr;
static char _duckyPayload[2048];

// --------------------------------------------------------------------------
// Server callbacks
// --------------------------------------------------------------------------
class ServerCB : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* s, NimBLEConnInfo& info) override {
    _connected   = true;
    _advertising = false;
    Serial.println("[BLE HID] host connected");
  }
  void onDisconnect(NimBLEServer* s, NimBLEConnInfo& info, int reason) override {
    _connected = false;
    Serial.println("[BLE HID] host disconnected, re-advertising");
    // Re-advertise so the host can reconnect after going out of range
    if (_server) _server->startAdvertising();
    _advertising = true;
  }
};
static ServerCB _serverCB;

// --------------------------------------------------------------------------
// Low-level key send helpers
// --------------------------------------------------------------------------
static void kbRelease() {
  if (!_kbInput || !_connected) return;
  uint8_t rep[8] = {0};
  _kbInput->setValue(rep, 8);
  _kbInput->notify();
  delay(10);
}

static void kbPress(uint8_t mod, uint8_t code) {
  if (!_kbInput || !_connected) return;
  uint8_t rep[8] = {mod, 0x00, code, 0,0,0,0,0};
  _kbInput->setValue(rep, 8);
  _kbInput->notify();
}

static void kbWriteChar(char c) {
  KeyPress kp = asciiToHid(c);
  if (!kp.code) return;
  kbPress(kp.mod, kp.code);
  delay(8);
  kbRelease();
}

static void mouseMove(int8_t x, int8_t y) {
  if (!_mouseInput || !_connected) return;
  uint8_t rep[4] = {0x00, (uint8_t)x, (uint8_t)y, 0x00};
  _mouseInput->setValue(rep, 4);
  _mouseInput->notify();
  delay(10);
}

// --------------------------------------------------------------------------
// Ducky interpreter (runs inside FreeRTOS task)
// --------------------------------------------------------------------------
static void runDucky(const char* script) {
  const char* p = script;
  while (*p && _hidRunning) {
    const char* eol = p;
    while (*eol && *eol != '\n' && *eol != '\r') eol++;
    int lineLen = eol - p;
    if (lineLen == 0) { p = eol + 1; continue; }
    char line[512];
    int cl = lineLen < 511 ? lineLen : 511;
    memcpy(line, p, cl);
    line[cl] = '\0';
    int tl = strlen(line);
    while (tl > 0 && (line[tl-1]==' ' || line[tl-1]=='\t')) line[--tl] = '\0';

    if (strncasecmp(line, "REM", 3)==0 && (line[3]==' ' || !line[3])) {
      // skip comment

    } else if (strncasecmp(line, "STRING ", 7)==0) {
      for (const char* c = line+7; *c && _hidRunning; c++) kbWriteChar(*c);

    } else if (strncasecmp(line, "STRINGLN ", 9)==0) {
      for (const char* c = line+9; *c && _hidRunning; c++) kbWriteChar(*c);
      if (_hidRunning) { kbPress(0, 0x28); delay(8); kbRelease(); }

    } else if (strncasecmp(line, "DELAY ", 6)==0) {
      int ms = atoi(line+6);
      if (ms>0 && ms<=60000) {
        int w=0;
        while (_hidRunning && w<ms) { delay(50); w+=50; }
      }

    } else {
      // Key combo or literal text
      char scratch[512];
      strncpy(scratch, line, 511); scratch[511]='\0';
      uint8_t mod=0, keys[6]={0}; int ki=0;
      bool allKeys=true;
      char* tok = strtok(scratch, " \t");
      while (tok) {
        bool found=false;
        for (int i=0; NAMED_KEYS[i].name; i++) {
          if (strcasecmp(tok, NAMED_KEYS[i].name)==0) {
            mod |= NAMED_KEYS[i].mod;
            if (NAMED_KEYS[i].code && ki<6) keys[ki++]=NAMED_KEYS[i].code;
            found=true; break;
          }
        }
        if (!found && strlen(tok)==1) {
          // single printable char as key code (e.g. "GUI r")
          KeyPress kp = asciiToHid(tok[0]);
          if (kp.code) { mod |= kp.mod; if (ki<6) keys[ki++]=kp.code; found=true; }
        }
        if (!found) { allKeys=false; break; }
        tok = strtok(nullptr, " \t");
      }
      if (allKeys && ki>0) {
        // Real key combo
        uint8_t rep[8] = {mod,0,0,0,0,0,0,0};
        for (int i=0;i<ki;i++) rep[2+i]=keys[i];
        if (_kbInput && _connected) {
          _kbInput->setValue(rep, 8); _kbInput->notify();
          delay(60); kbRelease();
        }
      } else {
        // Fallback: type as literal text
        for (const char* c=line; *c && _hidRunning; c++) kbWriteChar(*c);
      }
    }
    p = eol;
    if (*p=='\r') p++;
    if (*p=='\n') p++;
  }
  kbRelease();
}

static void duckyTaskFn(void*) {
  // Wait up to 3s for host to be ready
  int w=0;
  while (!_connected && _hidRunning && w<3000) { delay(100); w+=100; }
  delay(300);
  do {
    runDucky(_duckyPayload);
    w=0;
    while (_duckyLoop && _hidRunning && w<300) { delay(50); w+=50; }
  } while (_duckyLoop && _hidRunning);
  _hidRunning = false;
  _duckyTask  = nullptr;
  vTaskDelete(nullptr);
}

static void jiggleTaskFn(void*) {
  mouseMove(3, 0); delay(30); mouseMove(-3, 0);
  while (_jiggling) {
    int w=0;
    while (_jiggling && w<_jiggleInterval) { delay(250); w+=250; }
    if (_jiggling) { mouseMove(4,0); delay(30); mouseMove(-4,0); }
  }
  _jiggleTask = nullptr;
  vTaskDelete(nullptr);
}

// --------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------
void begin(const char* name) {
  // Store the name — used for advertising
  if (name && name[0]) {
    strncpy(_devName, name, sizeof(_devName) - 1);
    _devName[sizeof(_devName) - 1] = '\0';
  }

  // NimBLE is pre-initialized by BleTools::init() at boot.
  // Do NOT call NimBLEDevice::init() again — it would be a no-op but
  // calling it from a different task with a different name is confusing.
  if (!NimBLEDevice::isInitialized()) {
    // Fallback if somehow called before BleTools::init()
    NimBLEDevice::init("ESPionage");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  }

  if (!_server) {
    _server = NimBLEDevice::createServer();
    _server->setCallbacks(&_serverCB, false);
    _server->advertiseOnDisconnect(false); // we handle it ourselves

    _hid = new NimBLEHIDDevice(_server);
    _hid->setManufacturer("Apple Inc.");
    _hid->setPnp(0x02, 0x05AC, 0x0267, 0x0110); // Apple Wireless Keyboard
    _hid->setHidInfo(0x00, 0x01);
    _hid->setReportMap((uint8_t*)hidReportMap, sizeof(hidReportMap));
    _hid->setBatteryLevel(85);

    // Create input report characteristics for keyboard (ID 1) and mouse (ID 2)
    _kbInput    = _hid->getInputReport(1);
    _mouseInput = _hid->getInputReport(2);

    _server->start();
  }

  // Set up advertising
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->setAppearance(HID_KEYBOARD);
  adv->addServiceUUID(_hid->getHidService()->getUUID());
  adv->setName(_devName);
  adv->start();
  _advertising = true;
  Serial.printf("[BLE HID] advertising as \"%s\" — pair from your device\n", _devName);
}

void stop() {
  _jiggling   = false;
  _hidRunning = false;
  _duckyLoop  = false;
  vTaskDelay(pdMS_TO_TICKS(400));
  if (_jiggleTask) { vTaskDelete(_jiggleTask); _jiggleTask = nullptr; }
  if (_duckyTask)  { vTaskDelete(_duckyTask);  _duckyTask  = nullptr; }
  kbRelease();
  if (_server) {
    _server->stopAdvertising();
    // Disconnect all clients
    for (auto& peer : _server->getPeerDevices()) {
      _server->disconnect(peer);
    }
  }
  _advertising = false;
  _connected   = false;
  Serial.println("[BLE HID] stopped");
}

void handleAction(const char* type, const char* payload, int interval, bool looping,
                  const char* name) {
  if (!strcmp(type, "stop")) { stop(); return; }

  // "advertise" — (re)start BLE advertising so a host can pair
  if (!strcmp(type, "advertise")) { begin(name); return; }

  if (!strcmp(type, "mouse")) {
    if (_jiggleTask) {
      _jiggling = false;
      vTaskDelay(pdMS_TO_TICKS(350));
      if (_jiggleTask) { vTaskDelete(_jiggleTask); _jiggleTask = nullptr; }
    }
    _jiggleInterval = max(5, min(300, interval)) * 1000;
    _jiggling = true;
    xTaskCreate(jiggleTaskFn, "blehid_jig", 2048, nullptr, 1, &_jiggleTask);
    return;
  }

  if (!strcmp(type, "hid")) {
    if (_duckyTask) {
      _hidRunning = false;
      vTaskDelay(pdMS_TO_TICKS(350));
      if (_duckyTask) { vTaskDelete(_duckyTask); _duckyTask = nullptr; }
    }
    kbRelease();
    strncpy(_duckyPayload, payload ? payload : "", sizeof(_duckyPayload)-1);
    _duckyPayload[sizeof(_duckyPayload)-1] = '\0';
    _duckyLoop  = looping;
    _hidRunning = true;
    xTaskCreate(duckyTaskFn, "blehid_ducky", 4096, nullptr, 1, &_duckyTask);
    return;
  }
}

bool isAdvertising() { return _advertising; }
bool isConnected()   { return _connected;   }
bool isActive()      { return _jiggling || _hidRunning; }

String getStatusJson() {
  String s = "{";
  s += "\"advertising\":"  + String(_advertising ? "true":"false") + ",";
  s += "\"connected\":"    + String(_connected   ? "true":"false") + ",";
  s += "\"jiggling\":"     + String(_jiggling    ? "true":"false") + ",";
  s += "\"hidRunning\":"   + String(_hidRunning  ? "true":"false") + ",";
  s += "\"name\":\"" + String(_devName) + "\"";
  s += "}";
  return s;
}

} // namespace BleHid
