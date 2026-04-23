#pragma once
#include <Arduino.h>

// BLE HID keyboard + mouse — lets a paired phone/laptop use ESPionage
// as a wireless keyboard and mouse, running the same Ducky-style scripts
// as the USB HID feature but over Bluetooth Classic (BLE).

namespace BleHid {
  // Start advertising as a BLE HID keyboard. Call stop() first if BLE scan
  // or spam is running — they share the NimBLE stack.
  // name: Bluetooth device name shown during pairing (default "ESPionage KB").
  void begin(const char* name = "ESPionage KB");
  void stop();

  // Dispatch a BLE HID action (same interface as UsbTools::handleAction).
  //   type    : "advertise" | "hid" | "mouse" | "stop"
  //   payload : Ducky-style script (hid)
  //   interval: jiggle interval seconds (mouse)
  //   looping : repeat HID payload
  //   name    : Bluetooth device name for "advertise" action
  void handleAction(const char* type, const char* payload,
                    int interval, bool looping,
                    const char* name = "ESPionage KB");

  bool isAdvertising();   // true while looking for a host to pair
  bool isConnected();     // true when a host is paired and connected
  bool isActive();        // jiggler or HID script running
  String getStatusJson(); // {"advertising":bool,"connected":bool,"jiggling":bool,"hidRunning":bool,"name":str}
}
