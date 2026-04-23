#pragma once
#include <ArduinoJson.h>
#include <WString.h>

namespace Activity {
  void init();
  // type: "info" | "capture" | "alert" | "spam" | "scan"
  void log(const String& type, const String& msg);
  String json();
  void clear();
}
