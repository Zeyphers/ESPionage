#pragma once
// Only active when USBMode=default (ARDUINO_USB_MODE=0), i.e. TinyUSB is enabled.
#if defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE == 0

#include <Arduino.h>

namespace UsbTools {
  // Call once in setup() — registers keyboard + mouse with TinyUSB and starts USB
  void begin();

  // Dispatch a USB action from /api/usb
  //   type    : "hid" | "mouse" | "stop" | "spoof"
  //   payload : Ducky-style script (for type=="hid")
  //   interval: jiggle interval seconds (for type=="mouse")
  //   looping : repeat HID payload continuously
  //   os      : "windows" | "mac" (affects key combo selection, informational)
  //   vid/pid : hex strings for spoof
  void handleAction(const char* type, const char* payload,
                    int interval, bool looping,
                    const char* os,
                    const char* vid, const char* pid);

  void stop();                // stop all active USB actions
  bool isMounted();           // true when USB host has enumerated us
  bool isActive();            // true when jiggler or HID script is running
  String getStatusJson();     // {"mounted":true,"jiggling":false,"hidRunning":false}
  bool     uploadChunk(const char* filename, const uint8_t* data, size_t len, size_t index, bool final);
  String   listFilesJson();
  uint8_t* getFileData(const char* filename, size_t& outSize); // caller must free()
  bool     deleteFile(const char* filename);
  void     tick();            // call from loop() — handles deferred media-change signal
}

#endif // ARDUINO_USB_MODE == 0
