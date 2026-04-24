#pragma once
#include <ArduinoJson.h>
#include <WString.h>

namespace WifiTools {
  void init();

  // WiFi scan
  void startScan();
  void stopScan();
  void tickScan();
  String scanResultsJson();
  void fillStatus(JsonDocument& doc);

  // Beacon spam — broadcasts fake APs
  void startBeaconSpam(const JsonDocument& params);
  void stopBeaconSpam();
  void tickBeaconSpam();
  void fillBeaconStatus(JsonDocument& doc);

  // Probe sniffer — captures probe requests from nearby phones
  void startProbeSniff();
  void stopProbeSniff();
  void tickProbeSniff();
  String probesJson();
  void fillProbeStatus(JsonDocument& doc);

  // Wardrive — continuously logs APs with location-agnostic CSV
  void startWardrive();
  void stopWardrive();
  void tickWardrive();
  String wardriveCsv();
  void fillWardriveStatus(JsonDocument& doc);

  // WiFi Finder — live RSSI + estimated distance for a target AP
  void startFinder(const JsonDocument& params); // {bssid, channel}
  void stopFinder();
  void tickFinder();
  void finderCalibrate();       // sample current rssi as 1m reference
  void finderSetEnv(float n);   // path loss exponent
  void fillFinderStatus(JsonDocument& doc);

  // Deauth attack — disconnect clients from a target AP
  void startDeauth(const JsonDocument& params); // {bssid, channel}
  void stopDeauth();
  void tickDeauth();
  void fillDeauthStatus(JsonDocument& doc);

  // Rogue AP — broadcast a single named AP (open)
  void startRogueAp(const JsonDocument& params); // {ssid, channel}
  void stopRogueAp();
  void fillRogueStatus(JsonDocument& doc);

  // SSID Message — broadcast a list of SSIDs that form a message
  void startSsidMessage(const JsonDocument& params); // {text}
  void stopSsidMessage();

  // Radio control
  void setTxPower(int dbm);       // 2..20 ~dBm
  void randomizeMac();            // pick a new random STA MAC
  int  currentTxPower();

  // Clear all captured logs
  void clearLogs();

  // Push latest phone-GPS fix so wardrive can stamp entries
  void setGps(double lat, double lon, float acc);

  // Returns true (and clears the flag) if wardrive logged a new AP since last GPS update
  bool takeGpsRequest();
}
