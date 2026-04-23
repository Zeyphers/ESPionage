#pragma once
#include <ArduinoJson.h>
#include <WString.h>

namespace BleTools {
  enum SpamVendor { SPAM_APPLE, SPAM_GOOGLE, SPAM_SAMSUNG, SPAM_MICROSOFT, SPAM_MIX };

  void init();

  // BLE scan
  void startScan();
  void stopScan();
  void tickScan();
  String scanResultsJson();
  void fillStatus(JsonDocument& doc);

  // BLE spam (Apple/Google/Samsung/Microsoft/Mix pairing popups)
  // appleIdx: -1 = random rotation, 0..N = specific device index
  void startSpam(SpamVendor v, const JsonDocument& params, int appleIdx = -1);
  void stopSpam();
  void tickSpam();
  void updateAppleIdx(int idx);  // hot-swap device while spam is running
  void fillSpamStatus(JsonDocument& doc);

  // Returns JSON array of Apple device names for the UI picker
  String appleDeviceListJson();

  // AirTag / Find My detector
  void startAirtagDetect();
  void stopAirtagDetect();
  void tickAirtagDetect();
  void fillAirtagStatus(JsonDocument& doc);

  // BLE Finder — track a target MAC's RSSI and estimated distance
  void startFinder(const JsonDocument& params); // {mac}
  void stopFinder();
  void tickFinder();
  void finderCalibrate();
  void finderSetEnv(float n);
  void fillFinderStatus(JsonDocument& doc);

  // Custom advertiser — makes the ESP32 show up as any name
  void startCustomAdv(const JsonDocument& params); // {name}
  void stopCustomAdv();
  void fillCustomAdvStatus(JsonDocument& doc);

  // AirTag emulation — broadcast FindMy advertisements to trigger iOS alerts
  void startAirtagEmulation(const JsonDocument& params); // {name?, interval?}
  void stopAirtagEmulation();
  void tickAirtagEmulation();
  void fillAirtagEmulationStatus(JsonDocument& doc);

  // BLE skimmer detector — flag scan results matching known skimmer patterns
  void startSkimmerDetect();
  void stopSkimmerDetect();
  String skimmerJson();
  void fillSkimmerStatus(JsonDocument& doc);

  // GATT enumerator — connect to a device and list services/characteristics
  void startGattEnum(const JsonDocument& params); // {mac}
  void stopGattEnum();
  void tickGattEnum();
  String gattJson();
  void fillGattStatus(JsonDocument& doc);

  void clearLogs();
}
