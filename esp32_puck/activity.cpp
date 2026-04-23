#include "activity.h"
#include "config.h"
#include <vector>

namespace Activity {

struct Entry {
  uint32_t ts;
  String type;
  String msg;
};

static std::vector<Entry> entries;

void init() {
  entries.reserve(MAX_ACTIVITY_LOG);
}

void log(const String& type, const String& msg) {
  if (entries.size() >= MAX_ACTIVITY_LOG) entries.erase(entries.begin());
  Entry e;
  e.ts = millis() / 1000;
  e.type = type;
  e.msg = msg;
  entries.push_back(e);
  Serial.printf("[%s] %s\n", type.c_str(), msg.c_str());
}

String json() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  // Newest first
  for (auto it = entries.rbegin(); it != entries.rend(); ++it) {
    JsonObject o = arr.add<JsonObject>();
    o["ts"] = it->ts;
    o["type"] = it->type;
    o["msg"] = it->msg;
  }
  String out;
  serializeJson(doc, out);
  return out;
}

void clear() { entries.clear(); }

} // namespace Activity
