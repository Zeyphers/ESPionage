#!/usr/bin/env bash
# ESPionage — build & flash via arduino-cli
# Target: ESP32-S3-DevKitC-1 N16R8 (16MB flash, 8MB OPI PSRAM)
#
# HARDWARE CONVENTION (two USB-C ports on the DevKit):
#   • UART / COM port  → CH343/CP2102 bridge → ESP32-S3 UART0 (GPIO43/44)
#                        ALWAYS used for flashing + Serial debugging.
#                        Shows up on macOS as /dev/cu.usbserial-* or similar.
#   • USB port         → ESP32-S3 native USB OTG (GPIO19/20)
#                        Dedicated to HID (Apple Keyboard / mouse / ducky).
#                        Shows up as /dev/cu.usbmodem* only in ROM boot mode.
#
# With USBMode=default the app never touches UART0, so UART-port flashing
# works instantly without pressing BOOT or running over WiFi.
#
# Usage: ./flash.sh [port]   e.g.  ./flash.sh /dev/cu.usbserial-110

set -euo pipefail

SKETCH_DIR="esp32_puck"
FQBN="esp32:esp32:esp32s3:FlashSize=16M,PartitionScheme=min_spiffs,PSRAM=opi,USBMode=default,UploadSpeed=921600"
BUILD_DIR="$(pwd)/build"
AP_IP="192.168.4.1"

# ---- Dependency check --------------------------------------------------------
if ! command -v arduino-cli &>/dev/null; then
  echo "❌  arduino-cli not found. Install: brew install arduino-cli"
  exit 1
fi

# ---- Required libraries ------------------------------------------------------
echo "📦  Checking libraries…"
arduino-cli lib install "ArduinoJson"    2>/dev/null || true
arduino-cli lib install "NimBLE-Arduino" 2>/dev/null || true
# ESP_Async_WebServer 3.x + Async_TCP 3.x are pre-installed.
# Do NOT install the registry "AsyncTCP" 1.1.4 — it conflicts with Async_TCP 3.x.

# ---- Build -------------------------------------------------------------------
echo "🔨  Compiling for ESP32-S3 N16R8…"
mkdir -p "$BUILD_DIR"
arduino-cli compile \
  --fqbn "$FQBN" \
  --build-path "$BUILD_DIR" \
  "$SKETCH_DIR"

# ---- Port detection ----------------------------------------------------------
# Priority:
#   1. Explicit port argument
#   2. UART bridge port  (preferred — always flashable, doesn't disturb HID)
#   3. Native USB port   (only usable when in ROM bootloader mode)
if [[ ${1:-} != "" ]]; then
  PORT="$1"
  PORT_TYPE="explicit"
else
  UART_PORT=$(ls /dev/cu.usbserial-* /dev/cu.SLAB_USBtoUART* /dev/cu.wchusbserial* 2>/dev/null | head -1 || true)
  USB_PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -1 || true)

  if [[ -n "$UART_PORT" ]]; then
    PORT="$UART_PORT"
    PORT_TYPE="UART"
  elif [[ -n "$USB_PORT" ]]; then
    PORT="$USB_PORT"
    PORT_TYPE="native USB (ROM mode)"
  else
    PORT=""
    PORT_TYPE=""
  fi
fi

# ---- Native-USB fallback: trigger ROM mode over WiFi --------------------------
# If we only see the native USB port, it means the app is already running
# as a HID device and the port won't respond to esptool.  Kick it into
# ROM download mode via the /api/reboot-bootloader endpoint, then re-scan.
if [[ -z "$PORT" || "$PORT_TYPE" == "native USB (ROM mode)" ]]; then
  echo ""
  echo "⚠️   No UART port detected. Plug a USB-C cable into the UART/COM port"
  echo "    on the DevKit (the one that's NOT the native USB port) for the"
  echo "    easiest flashing experience."
  echo ""
  echo "    Falling back to native-USB path: attempting to trigger ROM mode"
  echo "    over WiFi (requires connection to ESPionage network)…"
  if curl -s --max-time 3 -X POST "http://$AP_IP/api/reboot-bootloader" | grep -q '"ok":true'; then
    echo "✅  Bootloader triggered via WiFi — waiting for port to reappear…"
    sleep 4
    PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -1 || true)
    PORT_TYPE="native USB (ROM mode, triggered via WiFi)"
  fi
fi

if [[ -z "$PORT" ]]; then
  echo "❌  No flashable port found."
  echo "    Try one of:"
  echo "      • Plug the UART/COM port into your Mac (recommended)"
  echo "      • Hold BOOT, tap RESET, release BOOT — then re-run"
  echo "      • Connect to ESPionage WiFi, then re-run"
  exit 1
fi

echo "📍  Port: $PORT ($PORT_TYPE)"

# ---- Upload ------------------------------------------------------------------
echo "⚡  Flashing…"
arduino-cli upload \
  --fqbn "$FQBN" \
  --port "$PORT" \
  --input-dir "$BUILD_DIR" \
  "$SKETCH_DIR"

echo ""
echo "✅  Done! Connect to WiFi:"
echo "    SSID: ESPionage"
echo "    Pass: espionage123"
echo "    URL:  http://192.168.4.1"
