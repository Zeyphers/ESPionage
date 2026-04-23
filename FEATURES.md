# ESPionage — Feature List

Organized by category. Checkmarks indicate what's implemented in this firmware.

---

## 📡 WiFi

### Implemented

- **WiFi Scanner** — passive scan of nearby access points with SSID, BSSID, channel, RSSI, encryption type
- **Beacon Spam** — broadcasts fake WiFi networks. Use a list of custom SSIDs, or generate random ones. Flood mode creates hundreds of fake APs
- **Rickroll AP** — creates an AP with a custom name (default: "Never Gonna Give You Up")
- **Evil Portal** — fake captive portal. Connect to the AP and get served a custom login page (default: fake Google/Facebook/router login). Captured credentials logged
- **Probe Sniffer** — passively captures probe requests from nearby phones to see what networks they're searching for (reveals home/work WiFi names of passing devices)
- **Wardriver** — silently logs every AP seen, with timestamps and signal strength, dumped to internal storage as CSV

### Stubbed (needs more work)

- **Deauth Attack** — requires raw 802.11 frame injection via esp-idf. Stub in place, needs implementation
- **PMKID Capture** — uses existing Marauder approach, needs WPA handshake parsing
- **Karma Attack** — respond to probe requests by creating matching AP
- **Handshake Capture** — capture WPA2 4-way handshakes

---

## 🔵 Bluetooth / BLE

### Implemented

- **BLE Scanner** — lists all nearby Bluetooth devices with name, MAC, RSSI, services
- **Apple BLE Spam (AppleJuice)** — broadcasts fake Apple device advertisements. Triggers the "pairing popup" on nearby iPhones. Supported devices:
  - AirPods Pro, AirPods Max
  - Apple TV (pairing, home, wireless audio sync, autofill)
  - Apple Vision Pro
  - BeatsX, Beats Solo
  - HomePod, HomePod Mini
- **Google Fast Pair Spam** — triggers Android "Nearby Device" popups. Supported devices:
  - Pixel Buds
  - Bose QC35, JBL Flip
  - Sony WH-1000XM series
- **Samsung BLE Spam** — triggers Galaxy Buds/Watch pairing popups
- **Microsoft Swift Pair Spam** — triggers Windows pairing popup for Surface devices
- **BLE Device Tracker** — logs BLE devices by MAC with timestamps & RSSI, useful for tracking presence
- **AirTag Detector** — scans for known Apple Find My beacons and alerts if any follow you (anti-stalking tool)

### Stubbed

- **AirTag Emulator** — broadcasts Apple Find My advertisements. Needs ECC P-224 keypair from OpenHaystack. Skeleton in place
- **BLE Jammer** — spam advertisements on all channels to disrupt nearby BLE
- **Bluetooth Classic spoofing** — needs BT classic stack

---

## 🎭 Spoofing & Emulation

- **MAC Spoofer** — randomize the ESP32's WiFi/BLE MAC
- **Random Device Name** — randomize BLE advertising name on each boot
- **Custom Beacon Frames** — craft custom 802.11 beacon frames with arbitrary SSID and parameters

---

## 🕵️ Surveillance / Recon

- **Packet Monitor** — real-time counter of management/data/deauth/probe packets on the current channel
- **Channel Hopper** — automatically hop through 2.4GHz channels 1-14 while scanning
- **BLE RSSI Proximity** — lock onto a specific BLE MAC and show live distance estimate via RSSI

---

## 🤖 Fun / Misc

- **Rickroll Portal** — evil portal that redirects to Rick Astley YouTube video
- **Fake Update Screen** — serves a "Windows Update" or "Apple Software Update" page
- **QR Code Generator** — generate QR codes for WiFi join, vCard, URLs (displayed in the web UI)
- **ESP-NOW Chat** — mesh chat between multiple ESPionage devices in range (no WiFi needed)
- **Wake-on-LAN** — send magic packets to wake up computers by MAC

---

## 📁 Data Management

- **Log Viewer** — view wardriving logs, probe captures, credentials in the web UI
- **Log Download** — download logs as CSV/JSON
- **Log Clear** — wipe logs from flash
- **Factory Reset** — clear all settings and logs

---

## ⚙️ System

- **WiFi Config** — change the ESP32's AP SSID, password, channel
- **Station Mode** — optionally connect to your home WiFi instead of AP mode (access from same network)
- **OTA Updates** — flash new firmware over the air via the web UI
- **Device Info** — chip info, flash size, free heap, uptime, MAC addresses
- **Restart / Reboot**
- **Theme** — light/dark mode for the web UI

---

## Legal / Ethical

Many of these features (deauth, evil portal, BLE spam) are in legal gray areas in some jurisdictions. Use only on your own equipment or with explicit permission. This firmware is for education and authorized security testing.
