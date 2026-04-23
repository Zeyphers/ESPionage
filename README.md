<p align="center">
  <img src="logo.png" alt="ESPionage" width="800"/>
</p>

<p align="center">
  A self-contained WiFi &amp; Bluetooth pen-test toolkit for the ESP32-S3.<br/>
  No laptop required — hosts its own WiFi AP and full-featured web UI accessible from any phone or browser.
</p>

<p align="center">
  <strong>For authorized security research and educational use only.</strong>
</p>

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

### Step 1 — Install Arduino IDE

Download and install **[Arduino IDE 2](https://www.arduino.cc/en/software)** if you don't have it already.

---

### Step 2 — Add the ESP32 board package

1. Open Arduino IDE → **File → Preferences**
2. Paste this URL into the **"Additional boards manager URLs"** box:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**, search for `esp32`, and install **esp32 by Espressif Systems** (version 3.x)

---

### Step 3 — Install the required libraries

Go to **Tools → Manage Libraries** and install each of these:

| Library | Search for |
|---|---|
| NimBLE-Arduino | `NimBLE-Arduino` by h2zero |
| ESPAsyncWebServer | `ESPAsyncWebServer` by lacamera |
| AsyncTCP | `AsyncTCP` by dvarrel |
| ArduinoJson | `ArduinoJson` by Benoit Blanchon |

---

### Step 4 — Open the sketch

Download or clone this repo, then open **`esp32_puck/esp32_puck.ino`** in Arduino IDE.

---

### Step 5 — Configure the board settings

Go to **Tools** and set the following:

| Setting | Value |
|---|---|
| Board | `ESP32S3 Dev Module` |
| USB Mode | `USB-OTG (TinyUSB)` |
| Flash Size | `16MB` *(adjust if your board has less)* |
| Partition Scheme | `16MB Flash (3MB APP/9.9MB FATFS)` |
| PSRAM | `OPI PSRAM` |
| CPU Frequency | `240MHz` |
| Upload Speed | `921600` |

> If you don't see these options make sure **ESP32S3 Dev Module** is selected under **Tools → Board → esp32**.

---

### Step 6 — Flash it

Plug in your ESP32-S3 via USB and click **Upload** (the → arrow button).

> **Device not showing up as a port?** Hold the **BOOT** button on the board, plug in the USB cable, then release BOOT. It should appear as a serial port.

---

### Step 7 — Connect and use

1. Once flashed, the ESP32 will boot and create a WiFi hotspot
2. On your phone or laptop, connect to **`ESPionage`** (password: `espionage123`)
3. Open a browser and go to **`http://192.168.4.1`**
4. The full UI loads — no app install needed

> You can change the AP name and password under the **System** tab. Settings are saved across reboots.

---

### GPS Support (iPhone / iPad)

The wardriving and GPS features work by having your phone push its location to the device. An **iOS Shortcut** is available that does this automatically while you drive:

**[📍 Download the ESPionage GPS Shortcut](https://www.icloud.com/shortcuts/d88a14e45a92426c98f01b5011200728)**

Once installed, run the shortcut while connected to the ESPionage hotspot. It will continuously send your phone's GPS coordinates to the device for tagging wardrive logs.

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
