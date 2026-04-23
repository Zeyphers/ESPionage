#pragma once

#define FIRMWARE_VERSION "0.3.0"

// Default AP credentials — can be changed at runtime from the web UI
#define AP_SSID_DEFAULT "ESPionage"
#define AP_PASS_DEFAULT "espionage123"

// Max items cached in various scanners before we drop old entries
#define MAX_WIFI_RESULTS 64
#define MAX_BLE_RESULTS 64
#define MAX_PROBE_RESULTS 1024
#define MAX_WARDRIVE_ENTRIES 256
#define MAX_PORTAL_CREDS 32
#define MAX_ACTIVITY_LOG 64

// Finder defaults (RSSI -> distance path loss model)
#define FINDER_MEASURED_POWER_WIFI -45   // dBm at 1m (callibrateable)
#define FINDER_MEASURED_POWER_BLE  -59   // dBm at 1m
#define FINDER_PATH_LOSS_N         3.0   // indoor home default
