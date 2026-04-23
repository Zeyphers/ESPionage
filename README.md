# ESPionage

A self-contained WiFi & Bluetooth pen-test toolkit for the ESP32-S3. No laptop required — the device hosts its own WiFi access point and a full-featured web UI accessible from any phone or browser.

> **For authorized security research and educational use only.**

---

## Compatible Hardware

| Board | Status | Notes |
|---|---|---|
| **ESP32-S3 DevKitC-1 (N16R8)** | ✅ Primary target | 16MB flash, 8MB PSRAM — fully supported |
| ESP32-S3 DevKitC-1 (N8R8) | ✅ Supported | 8MB flash — all features work |
| ESP32-S3-WROOM-1 (any) | ✅ Supported | Ensure USB OTG is available for USB HID features |
| ESP32-S2 | ⚠️ Partial | No Bluetooth — WiFi features only, no USB HID |
| ESP32 (classic WROOM) | ⚠️ Partial | No USB HID, no PSRAM for MSC. BLE spam works |
| ESP32-C3 / C6 | ❌ Untested | Different USB stack — may not compile as-is |

> The USB HID, USB mass storage, and USB device spoofing features require an **ESP32-S3** with native USB OTG (USB Mode = USB-OTG / TinyUSB).

---

## Features

### 📡 WiFi
| Feature | Description |
|---|---|
| **WiFi Scanner** | Passive scan — SSID, BSSID, channel, RSSI, encryption |
| **Beacon Spam** | Flood the airspace with fake SSIDs (custom list or random) |
| **SSID Message** | Broadcast a message as a single beacon |
| **Evil Portal** | Rogue captive portal with credential harvesting (Google, Facebook, router templates) |
| **Rogue AP** | Open access point — capture traffic from connecting clients |
| **Probe Sniffer** | Capture probe requests from nearby phones — reveals networks they know |
| **Deauth Attack** | 802.11 deauthentication frame injection |
| **Wardriving** | Silent AP logging with timestamps and GPS (phone-supplied) |
| **WiFi Finder** | RSSI-based distance estimator for a specific AP |
| **TX Power Control** | Adjust transmit power from the UI |
| **MAC Randomizer** | Randomize the ESP32's WiFi MAC |

### 🔵 Bluetooth / BLE
| Feature | Description |
|---|---|
| **BLE Scanner** | List nearby devices with name, MAC, RSSI, manufacturer |
| **Apple Juice (BLE Spam)** | iOS 18 compatible — proximity pairing popup + action modal. 39 device types |
| **Google Fast Pair Spam** | Android "Nearby Device" pairing popups — 10 device profiles |
| **Samsung EasySetup Spam** | Galaxy Buds / SmartTag pairing notifications |
| **Microsoft Swift Pair Spam** | Windows Bluetooth device popup |
| **Mixed BLE Spam** | Cycles through all four vendors simultaneously |
| **AirTag Emulator** | Broadcast Apple FindMy advertisements to appear on Apple Maps |
| **AirTag Detector** | Identify Apple AirTag beacons nearby (anti-stalking) |
| **BLE Finder** | RSSI-based distance estimator for a specific BLE device |
| **Custom BLE Advertiser** | Broadcast arbitrary advertisement data with a custom name |
| **BLE HID Keyboard** | Pair as a wireless Bluetooth keyboard — run DuckyScript payloads |
| **BLE Mouse Jiggler** | Keep a computer awake wirelessly by simulating mouse movement |

### 🖥️ USB (ESP32-S3 only)
| Feature | Description |
|---|---|
| **USB HID Keyboard** | Wired USB keyboard — execute DuckyScript payloads |
| **USB Mouse Jiggler** | Simulate mouse movement to prevent screen lock |
| **USB Device Spoof** | Reboot with custom VID/PID to impersonate any USB device |
| **USB Mass Storage** | Present a 512KB RAM disk to the host via USB MSC |
| **Reboot to Bootloader** | One-click entry to USB download mode for reflashing |

### 🌐 Portal & Capture
| Feature | Description |
|---|---|
| **Evil Portal** | Customisable captive portal — templates for Google, Facebook, router login |
| **Credential Log** | Captured usernames/passwords viewable in the UI |
| **GPS Support** | Phone's browser geolocation pushed to the device for wardrive tagging |

### ⚙️ System
| Feature | Description |
|---|---|
| **Web UI** | Mobile-first, no-install browser interface at `http://192.168.4.1` |
| **WebSocket Status** | Real-time mode stats pushed to the browser |
| **Activity Log** | Timestamped log of all actions and events |
| **AP Config** | Change SSID, password, TX power — persisted across reboots |
| **Conflict System** | Warns when two features share a radio (BLE HID + BLE spam, etc.) |
| **Device Info** | Chip model, cores, frequency, flash, heap, MAC |

---

## Installation

### Prerequisites

- **Arduino IDE 2.x** or **arduino-cli**
- **ESP32 Arduino Core 3.x** — add the board manager URL:
  ```
  https://espressif.github.io/arduino-esp32/package_esp32_index.json
  ```
- **Required libraries** (install via Library Manager or manually):
  | Library | Author |
  |---|---|
  | `NimBLE-Arduino` | h2zero |
  | `ESPAsyncWebServer` | lacamera / dvarrel |
  | `AsyncTCP` | dvarrel |
  | `ArduinoJson` | Benoit Blanchon |

---

### Arduino IDE

1. Clone or download this repository
2. Open `esp32_puck/esp32_puck.ino` in Arduino IDE
3. Select your board and configure:

   | Setting | Value |
   |---|---|
   | Board | `ESP32S3 Dev Module` |
   | USB Mode | `USB-OTG (TinyUSB)` |
   | Flash Size | `16MB` (or match your board) |
   | Partition Scheme | `16MB Flash (3MB APP / 9.9MB FATFS)` |
   | PSRAM | `OPI PSRAM` |
   | CPU Frequency | `240MHz` |

4. Click **Upload**

---

### arduino-cli (one-liner)

```bash
arduino-cli compile --fqbn esp32:esp32:esp32s3:USBMode=default,FlashSize=16M,PartitionScheme=app3M_fat9M_16MB,CPUFreq=240,PSRAM=opi esp32_puck && \
arduino-cli upload  --fqbn esp32:esp32:esp32s3:USBMode=default,FlashSize=16M,PartitionScheme=app3M_fat9M_16MB,CPUFreq=240,PSRAM=opi \
  -p /dev/cu.usbmodem* esp32_puck
```

---

### First Boot

1. Power the ESP32 — it will create a WiFi access point
2. Connect to **`ESPionage`** (default password: `espionage123`)
3. Open **`http://192.168.4.1`** in any browser
4. The full UI loads — no app install required

> You can change the AP name and password from the **System** tab. Changes persist across reboots.

---

### Entering Download Mode (if the device isn't showing up as a serial port)

Hold **BOOT**, plug in the USB cable, then release **BOOT**. The device will enumerate as a serial port for flashing.

---

## Usage Notes

- **BLE scan** runs in 8-second bursts with a 2-second pause between them. This prevents NimBLE from starving the WiFi stack on Core 0. Devices accumulate across bursts — results are not cleared between cycles.
- **BLE spam and BLE HID** share the same radio — the conflict banner will warn you if both are active.
- **Evil Portal / Rogue AP** take over the WiFi radio — you can still reach the UI via the existing WiFi connection but scanning won't work while they're running.
- **USB features** require the ESP32-S3 to be connected as a USB device to a host computer (separate from your flashing connection if using a DevKit with two ports).

---

## Credits & Acknowledgements

ESPionage is built on the work of a lot of people in the security research community:

| Project | Author | Used for |
|---|---|---|
| [ESP32 Marauder](https://github.com/justcallmekoko/ESP32Marauder) | justcallmekoko | Architecture inspiration, WiFi attack patterns |
| [EvilAppleJuice-ESP32](https://github.com/ckcr4lyf/EvilAppleJuice-ESP32) | ckcr4lyf | iOS 18 compatible Apple BLE packet format |
| [iOS-BLE-Proximity-Attack-ESP32](https://github.com/l33tfg/iOS-BLE-Proximity-Attack-ESP32) | l33tfg | Random MAC + ADV type technique for iOS 18 |
| [AppleJuice](https://github.com/ECTO-1A/AppleJuice) | ECTO-1A, ronaldstoner | Original Apple BLE spam concept |
| [Bluetooth-LE-Spam](https://github.com/simondankelmann/Bluetooth-LE-Spam) | simondankelmann | Action modal action codes (iOS 16/17/18 confirmed) |
| [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) | h2zero | BLE stack (scan, advertise, HID, GATT) |
| [ESPAsyncWebServer](https://github.com/lacamera/ESPAsyncWebServer) | lacamera / Me-No-Dev | Async HTTP + WebSocket server |
| [ArduinoJson](https://github.com/bblanchon/ArduinoJson) | Benoit Blanchon | JSON serialization |
| [esp32-airtag](https://github.com/Guinn-Partners/esp32-airtag) | Guinn Partners | AirTag/FindMy advertisement payload reference |

---

## Legal

This firmware is intended for **authorized security research, penetration testing, and education** only.

- Use only on networks and devices you own or have explicit written permission to test
- BLE advertising attacks, deauthentication, and rogue portals may be illegal in your jurisdiction
- The authors accept no responsibility for misuse

<!-- ESPionage -->
