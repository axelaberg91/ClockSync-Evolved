#pragma once

// Wi-Fi setup, fallback and deferred configuration by axelaberg91.

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <DNSServer.h>
#include <esp_system.h>

static const char WIFI_SETUP_UI[] PROGMEM = R"WIFISETUP(
<!doctype html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>clocksync evolved · Wi-Fi</title>
<style>
:root{color-scheme:light}*{box-sizing:border-box}body{margin:0;background:#f4f5f6;color:#222b30;font:15px/1.5 system-ui,-apple-system,Segoe UI,sans-serif}main{max-width:540px;margin:40px auto;padding:0 18px}h1{font-size:27px;line-height:1.2;margin:28px 0 12px}p{color:#68747b;font-size:14px}.brand{font-size:20px;font-weight:700}section{background:#fff;border:1px solid #dce1e5;border-radius:10px;padding:24px;margin-top:24px}label{display:block;margin-top:16px;font-size:13px;font-weight:600}input[type=text],input[type=password]{width:100%;min-height:44px;margin-top:6px;padding:10px;border:1px solid #cbd3d9;border-radius:6px;font:inherit}input:focus-visible,button:focus-visible,a:focus-visible{outline:3px solid #8dbba8;outline-offset:3px}.show{display:flex;align-items:center;gap:8px;font-weight:400}button{width:100%;min-height:44px;margin-top:22px;border:0;border-radius:7px;background:#222b30;color:#fff;font:inherit;font-weight:600;cursor:pointer}button:disabled{opacity:.5;cursor:wait}#result{min-height:44px;margin:16px 0 0;font-size:14px}#result.error{color:#a33832}#result.ok{color:#17694b}.note{font-size:12px}#destination{display:block;margin-top:10px;color:#17694b;overflow-wrap:anywhere}footer{font-size:12px;color:#68747b;margin-top:20px}
</style></head><body><main>
<div class="brand">clocksync evolved</div><h1>Wi-Fi settings</h1>
<p>Enter the network clocksync evolved should connect to. The network must support 2.4 GHz.</p>
<section><form id="wifiForm">
<label for="ssid">Network name (SSID)</label><input id="ssid" name="ssid" type="text" maxlength="32" required autocomplete="off" autocapitalize="none" spellcheck="false">
<label for="psk">Password (PSK)</label><input id="psk" name="psk" type="password" minlength="8" maxlength="64" autocomplete="new-password" autocapitalize="none" spellcheck="false">
<label class="show"><input id="showPassword" type="checkbox">Show password</label>
<p class="note">Leave the password empty for an open network. Settings are saved to the device once the connection succeeds.</p>
<button id="save" type="submit" disabled>Connect and save</button>
</form><p id="result" role="status" aria-live="polite">Loading connection status…</p><a id="destination" hidden></a></section>
<footer>Setup network: clocksync · Address: 192.168.4.1</footer>
</main><script>
(function(){
'use strict';
var token='',busy=false,connected=false,loaded=false,timer=null;
var el=function(id){return document.getElementById(id);};
function message(text,kind){el('result').textContent=text;el('result').className=kind||'';}
function api(url,options){var controller=new AbortController(),timeout=setTimeout(function(){controller.abort();},4000);options=options||{};options.signal=controller.signal;options.cache='no-store';return fetch(url,options).then(function(r){return r.json().then(function(s){if(!r.ok)throw new Error(s.message||'Request failed.');return s;});}).finally(function(){clearTimeout(timeout);});}
function show(s){
token=s.token||token;connected=s.state==='connected';
if(!loaded&&!el('ssid').value&&s.ssid)el('ssid').value=s.ssid;
loaded=true;
message(s.message,connected?'ok':s.state==='error'?'error':'');
el('save').disabled=busy||!token;
if(connected){el('destination').hidden=false;el('destination').textContent='http://'+s.ip;el('destination').href='http://'+s.ip;}
}
function refresh(){
clearTimeout(timer);if(document.hidden||busy)return;
api('/api/wifi').then(show).catch(function(error){
if(connected)message('The setup network has closed. Rejoin your normal Wi-Fi and open the address below.','ok');
else message(error.message||'Cannot reach the device. Check that you are connected to clocksync.','error');
}).finally(function(){if(!document.hidden)timer=setTimeout(refresh,2000);});
}
el('showPassword').addEventListener('change',function(){el('psk').type=this.checked?'text':'password';});
el('wifiForm').addEventListener('submit',function(ev){
ev.preventDefault();if(busy||!token)return;busy=true;connected=false;clearTimeout(timer);el('save').disabled=true;el('destination').hidden=true;
var form=new URLSearchParams(new FormData(ev.target));form.set('token',token);message('Sending settings…');
api('/api/wifi',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:form.toString()}).then(show).catch(function(error){message(error.message,'error');}).finally(function(){busy=false;el('save').disabled=!token;timer=setTimeout(refresh,2000);});
});
document.addEventListener('visibilitychange',function(){clearTimeout(timer);if(!document.hidden)refresh();});
refresh();
})();
</script></body></html>
)WIFISETUP";

class WiFiConfiguration {
 public:
  static constexpr uint32_t FALLBACK_MS = 10UL * 60 * 1000;
  static constexpr uint32_t RETRY_MS = 60000;
  static constexpr uint32_t CLOSE_AP_MS = 15000;

  explicit WiFiConfiguration(WebServer &server) : server_(server) {}

  void begin(const char *hostname, const char *compiledSSID = "", const char *compiledPSK = "") {
    hostname_ = hostname;
    makeToken(settingsToken_, sizeof(settingsToken_));
    if (!loadCredentials() && compiledSSID && validSSID(String(compiledSSID)) &&
        compiledPSK && validPSK(String(compiledPSK))) {
      ssid_ = compiledSSID;
      psk_ = compiledPSK;
    }
    nextSSID_ = ssid_;
    nextPortal_ = ssid_.length() == 0;
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setAutoReconnect(ssid_.length() > 0);
    WiFi.setHostname(hostname_.c_str());
    offlineSince_ = lastReconnect_ = millis();
    if (ssid_.length()) WiFi.begin(ssid_.c_str(), psk_.c_str());
    else { WiFi.disconnect(false, false); startPortal(); }
  }

  void registerRoutes() {
    server_.on("/api/wifi", HTTP_GET, [this]() {
      if (!portalClient()) { sendError(403, "Connect to the clocksync setup network first."); return; }
      sendStatus();
    });
    server_.on("/api/wifi", HTTP_POST, [this]() { acceptCredentials(); });
    server_.on("/api/wifi/next", HTTP_GET, [this]() {
      if (!settingsClient()) { sendError(403, "Open the device page through your normal Wi-Fi network."); return; }
      sendNextStatus();
    });
    server_.on("/api/wifi/next", HTTP_POST, [this]() { stageCredentials(); });
    server_.on("/api/wifi/reset", HTTP_POST, [this]() { stageReset(); });
    const char *probes[] = {"/generate_204", "/gen_204", "/hotspot-detect.html", "/connecttest.txt", "/ncsi.txt"};
    for (const char *path : probes) server_.on(path, HTTP_GET, [this]() {
      if (!portalClient()) { server_.send(404, "text/plain", "Not found"); return; }
      server_.sendHeader("Location", String("http://") + WiFi.softAPIP().toString() + "/");
      server_.send(302, "text/plain", "");
    });
  }

  bool portalClient() {
    return apActive_ && server_.client().localIP() == WiFi.softAPIP();
  }

  bool portalActive() const { return apActive_; }

  void service() {
    const uint32_t now = millis();
    if (apActive_) dns_.processNextRequest();
    if (WiFi.status() == WL_CONNECTED) {
      offlineSince_ = now;
      if (!wasConnected_) {
        Serial.print("Wi-Fi connected. http://"); Serial.println(WiFi.localIP());
        wasConnected_ = true;
        linkSince_ = now;
      }
      if (candidate_) {
        if (WiFi.SSID() != ssid_ || now - linkSince_ < 1000) return;
        saveFailed_ = !saveCredentials();
        candidate_ = false;
      }
      if (apActive_ && !saveFailed_) {
        if (!closingAP_) { closingAP_ = true; connectedSince_ = now; }
        if (now - connectedSince_ >= CLOSE_AP_MS) {
          dns_.stop();
          WiFi.softAPdisconnect(true);
          WiFi.mode(WIFI_STA);
          apActive_ = closingAP_ = false;
          Serial.println("Wi-Fi setup AP closed.");
        }
      }
      return;
    }
    if (wasConnected_) { offlineSince_ = now; wasConnected_ = false; }
    closingAP_ = false;
    if (ssid_.length() && now - lastReconnect_ >= RETRY_MS) {
      lastReconnect_ = now;
      WiFi.reconnect();
    }
    if (!apActive_ && now - offlineSince_ >= FALLBACK_MS &&
        (!portalAttempted_ || now - lastPortalAttempt_ >= RETRY_MS)) startPortal();
  }

  static bool validSSID(const String &s) {
    const int nullIndex = s.indexOf('\0');
    return s.length() > 0 && s.length() <= 32 && (nullIndex < 0 || (unsigned)nullIndex == s.length());
  }

  static bool validPSK(const String &s) {
    const int nullIndex = s.indexOf('\0');
    if (nullIndex >= 0 && (unsigned)nullIndex < s.length()) return false;
    if (s.length() == 0 || (s.length() >= 8 && s.length() <= 63)) return true;
    if (s.length() != 64) return false;
    for (unsigned i = 0; i < s.length(); ++i) if (!isxdigit((unsigned char)s[i])) return false;
    return true;
  }

 private:
  struct StoredNetwork {
    uint32_t magic;
    char ssid[33];
    char psk[65];
  };
  static constexpr uint32_t NETWORK_MAGIC = 0x57494631;
  static constexpr uint32_t PORTAL_MAGIC = 0x57494630;
  WebServer &server_;
  DNSServer dns_;
  String hostname_, ssid_, psk_, nextSSID_;
  char token_[33] = {};
  char settingsToken_[33] = {};
  bool apActive_ = false, wasConnected_ = false, candidate_ = false;
  bool closingAP_ = false, saveFailed_ = false, portalAttempted_ = false;
  bool nextPortal_ = false, nextPending_ = false;
  uint32_t offlineSince_ = 0, lastReconnect_ = 0, connectedSince_ = 0;
  uint32_t lastPortalAttempt_ = 0, submittedAt_ = 0;
  uint32_t linkSince_ = 0;

  bool loadCredentials() {
    Preferences storage;
    if (!storage.begin("clocksyncwifi", true)) return false;
    StoredNetwork n = {};
    const bool loaded = storage.getBytesLength("network") == sizeof(n) &&
                        storage.getBytes("network", &n, sizeof(n)) == sizeof(n);
    storage.end();
    if (!loaded) return false;
    // An explicit reset must suppress compiled secrets on every subsequent boot.
    if (n.magic == PORTAL_MAGIC) { ssid_ = ""; psk_ = ""; return true; }
    if (n.magic != NETWORK_MAGIC || n.ssid[32] || n.psk[64] ||
        !validSSID(String(n.ssid)) || !validPSK(String(n.psk))) return false;
    ssid_ = n.ssid; psk_ = n.psk;
    return true;
  }

  bool saveCredentials() {
    if (!storeCredentials(ssid_, psk_)) return false;
    nextSSID_ = ssid_; nextPortal_ = nextPending_ = false;
    return true;
  }

  bool storeCredentials(const String &ssid, const String &psk, bool portalOnly = false) {
    StoredNetwork n = {};
    n.magic = portalOnly ? PORTAL_MAGIC : NETWORK_MAGIC;
    if (!portalOnly) {
      ssid.toCharArray(n.ssid, sizeof(n.ssid));
      psk.toCharArray(n.psk, sizeof(n.psk));
    }
    // Keep boot mode and credentials in the same record, including explicit resets.
    Preferences storage;
    if (!storage.begin("clocksyncwifi", false)) return false;
    const bool saved = storage.putBytes("network", &n, sizeof(n)) == sizeof(n);
    storage.end();
    return saved;
  }

  static void makeToken(char *token, size_t size) {
    snprintf(token, size, "%08lx%08lx%08lx%08lx",
             (unsigned long)esp_random(), (unsigned long)esp_random(),
             (unsigned long)esp_random(), (unsigned long)esp_random());
  }

  bool settingsClient() {
    return WiFi.status() == WL_CONNECTED && server_.client().localIP() == WiFi.localIP();
  }

  bool settingsWriteAllowed() {
    if (!settingsClient()) { sendError(403, "Open the device page through your normal Wi-Fi network."); return false; }
    if (!server_.hasArg("token") || server_.arg("token") != settingsToken_) {
      sendError(403, "Reload the page and try again."); return false;
    }
    // Provisioning must finish before another request can replace its saved record.
    if (candidate_) { sendError(409, "Wi-Fi setup is still in progress. Wait for it to finish first."); return false; }
    return true;
  }

  void sendNextStatus() {
    const char *message = nextPending_ ?
        (nextPortal_ ? "Wi-Fi settings cleared. After the next restart, connect to the clocksync setup network. The current connection stays active." :
         "Network saved for the next startup. Restart the device when ready. These credentials have not been tested yet.") :
        "Changes apply after the next restart. The current connection stays active.";
    String response; response.reserve(700);
    response = "{\"currentSSID\":"; response += quoteJSON(ssid_);
    response += ",\"nextSSID\":"; response += quoteJSON(nextSSID_);
    response += ",\"forceAP\":"; response += nextPortal_ ? "true" : "false";
    response += ",\"pending\":"; response += nextPending_ ? "true" : "false";
    response += ",\"canEdit\":"; response += candidate_ ? "false" : "true";
    response += ",\"message\":"; response += quoteJSON(message);
    response += ",\"token\":"; response += quoteJSON(settingsToken_); response += '}';
    server_.sendHeader("Cache-Control", "no-store");
    server_.send(200, "application/json; charset=utf-8", response);
  }

  void stageCredentials() {
    if (!settingsWriteAllowed()) return;
    if (!server_.hasArg("ssid") || !server_.hasArg("psk")) { sendError(400, "SSID or password is missing."); return; }
    const String ssid = server_.arg("ssid"), psk = server_.arg("psk");
    if (!validSSID(ssid) || !validPSK(psk)) {
      sendError(400, "SSID must be 1-32 bytes. Password must be 8-63 bytes, 64 hexadecimal characters, or empty for an open network."); return;
    }
    if (!storeCredentials(ssid, psk)) { sendError(500, "Could not save Wi-Fi settings. Try again."); return; }
    // Keep the active connection and reconnect credentials untouched until restart.
    nextSSID_ = ssid; nextPortal_ = false; nextPending_ = true;
    sendNextStatus();
  }

  void stageReset() {
    if (!settingsWriteAllowed()) return;
    if (!storeCredentials(String(""), String(""), true)) { sendError(500, "Could not clear Wi-Fi settings. Try again."); return; }
    nextSSID_ = ""; nextPortal_ = nextPending_ = true;
    sendNextStatus();
  }

  void startPortal() {
    lastPortalAttempt_ = millis(); portalAttempted_ = true;
    WiFi.mode(WIFI_AP_STA);
    const IPAddress address(192, 168, 4, 1), subnet(255, 255, 255, 0);
    if (!WiFi.softAPConfig(address, address, subnet) || !WiFi.softAP("clocksync", "clocksync")) {
      Serial.println("Could not start Wi-Fi setup AP; retrying later.");
      return;
    }
    makeToken(token_, sizeof(token_));
    apActive_ = true;
    dns_.start(53, "*", address);
    Serial.println("Wi-Fi setup: SSID clocksync | password clocksync | http://192.168.4.1");
  }

  static String quoteJSON(const String &value) {
    String out = "\"";
    for (unsigned i = 0; i < value.length(); ++i) {
      const unsigned char c = value[i];
      if (c == '"' || c == '\\') { out += '\\'; out += (char)c; }
      else if (c < 32) { char escaped[7]; snprintf(escaped, sizeof(escaped), "\\u%04x", c); out += escaped; }
      else out += (char)c;
    }
    out += '"'; return out;
  }

  void sendError(int code, const char *message) {
    server_.sendHeader("Cache-Control", "no-store");
    server_.send(code, "application/json; charset=utf-8", String("{\"message\":") + quoteJSON(message) + "}");
  }

  void sendStatus() {
    const bool connected = WiFi.status() == WL_CONNECTED;
    const char *state = "ready", *message = "Enter the network name and password.";
    if (saveFailed_) { state = "error"; message = "Connected, but settings could not be saved. Try again."; }
    else if (connected && !candidate_) { state = "connected"; message = "Connected. The setup network will close within 15 seconds. Rejoin your normal Wi-Fi and open the address below."; }
    else if (candidate_) {
      state = "connecting";
      message = connected ? "Connected. Checking and saving settings…" :
          millis() - submittedAt_ < 30000 ? "Connecting to the network…" : "Not connected yet. Check the settings or try again. The setup network is still available.";
    }
    String response; response.reserve(650);
    response = "{\"state\":"; response += quoteJSON(state);
    response += ",\"message\":"; response += quoteJSON(message);
    response += ",\"ssid\":"; response += quoteJSON(ssid_);
    response += ",\"ip\":"; response += quoteJSON(connected ? WiFi.localIP().toString() : String(""));
    response += ",\"token\":"; response += quoteJSON(token_); response += '}';
    server_.sendHeader("Cache-Control", "no-store");
    server_.send(200, "application/json; charset=utf-8", response);
  }

  void acceptCredentials() {
    if (!portalClient()) { sendError(403, "Connect to the clocksync setup network first."); return; }
    if (!server_.hasArg("token") || server_.arg("token") != token_) { sendError(403, "Reload the page and try again."); return; }
    if (!server_.hasArg("ssid") || !server_.hasArg("psk")) { sendError(400, "SSID or password is missing."); return; }
    const String ssid = server_.arg("ssid"), psk = server_.arg("psk");
    if (!validSSID(ssid) || !validPSK(psk)) { sendError(400, "SSID must be 1-32 bytes. Password must be 8-63 bytes, 64 hexadecimal characters, or empty for an open network."); return; }
    ssid_ = ssid; psk_ = psk;
    candidate_ = true; saveFailed_ = closingAP_ = wasConnected_ = false;
    submittedAt_ = offlineSince_ = lastReconnect_ = millis();
    WiFi.setAutoReconnect(true);
    WiFi.disconnect(false, false);
    WiFi.begin(ssid_.c_str(), psk_.c_str());
    sendStatus();
  }
};
