#include "cover.h"

Cover::Cover(Cable& txt, Shell& exe, Store& ini)
: txt(txt), exe(exe), ini(ini) {}

void Cover::setup(void) {
    if (!WiFi.mode(WIFI_STA)) { this->txt.sos("wifi error", true); }
    this->apply();
    this->exe.add(this, &Cover::cmd_dialup, "wdial", "wifi dialup");
    this->exe.add(this, &Cover::cmd_hangup, "whang", "wifi hangup");
    this->exe.add(this, &Cover::cmd_lookup, "wlook", "mDNS lookup");
    this->exe.add(this, &Cover::cmd_status, "wstat", "wifi status");
}

bool Cover::check(bool quick) {
    if (WiFi.isConnected()) { return true; }
    else if(quick) { return false; }
    for (uint16_t tick = 2048; tick > 0; tick--) {
        switch(WiFi.status()) {
            case WL_DISCONNECTED:   this->txt.text("~");    break;
            case WL_IDLE_STATUS:    this->txt.text("-");    break;
            case WL_NO_SSID_AVAIL:
                this->txt.log("cover", "wifi ssid can't be reached!");
                return false;
            case WL_CONNECT_FAILED:
                this->txt.log("cover", "wifi password incorrect!");
                return false;
            case WL_CONNECTION_LOST:
                this->txt.log("cover", "wifi connection lost!");
                return false;
            case WL_SCAN_COMPLETED:
                this->txt.log("cover", "wifi scan completed.");
                break;
            case WL_CONNECTED:
                this->txt.log("cover", "wifi connected!");
                return true;
            default: break;
        }
        delay(16);
    }
    this->txt.text(" ... giving up!", true);
    return false;
}

bool Cover::hangup(void) {
    if (!WiFi.isConnected()) { return true; }
    this->txt.log("cover", "hangup");
    this->txt.llg("connected", "hang up..");
    WiFi.disconnect(true); delay(1024);
    if (WiFi.isConnected()) { return false; } else { this->hangups++; }
    return true;
}

bool Cover::apply(void) {
    String hostname = this->ini.get("hostname", COVER_H_NAME, true);
    String wifissid = this->ini.get("wifissid", COVER_W_SSID, true);
    String wifipass = this->ini.get("wifipass", COVER_W_PASS, true);
    this->hangup();
    this->txt.log("cover", "apply");
    this->txt.llg("hostname", hostname);
    this->txt.llg("ssid", wifissid);
    this->txt.llg("pass", this->txt.fill(wifipass.length(), '*'));
    WiFi.hostname(hostname);
    WiFi.setAutoReconnect(true);
    WiFi.begin(wifissid.c_str(), wifipass.c_str());
    if (!this->check(false)) { return false; } else { this->dialups++; }
    this->status();
    if (MDNS.begin(hostname.c_str(), WiFi.localIP())) {
        this->txt.llg("mDNS", "started");
        MDNS.addService("http", "tcp", SERVE_HARBOR);
        this->txt.llg("service", "registered");
    }
    return true;
}

void Cover::status(void) {
    this->txt.log("cover", "status");
    this->txt.llg("dialups", String(this->dialups));
    this->txt.llg("hangups", String(this->hangups));
    if (!WiFi.isConnected()) {
        this->txt.llg("status", "disconnected!"); return;
    }
    this->txt.llg("status", "connected!");
    this->txt.llg("ssid", WiFi.SSID());
    this->txt.llg("bssid", WiFi.BSSIDstr());
    this->txt.llg("mac", WiFi.macAddress());
    this->txt.llg("channel", "#", String(WiFi.channel()));
    this->txt.llg("signal", String(WiFi.RSSI()), " dBm");
    this->txt.llg(":",  WiFi.hostname(), " ::");
    this->txt.llg("address", WiFi.localIP().toString());
    this->txt.llg("subnet", WiFi.subnetMask().toString());
    this->txt.llg("gateway", WiFi.gatewayIP().toString());
    this->txt.llg("dns", WiFi.dnsIP().toString());
}
void Cover::lookup(void) {
    this->txt.log("cover", "lookup");
    if (!WiFi.isConnected()) {
        this->txt.llg("status", "disconnected!"); return;
    }
    int query = MDNS.queryService("http", "tcp");
    if (!query) { this->txt.llg("no results", "sorry"); return; }
    this->txt.llg("results", String(query));
    for (uint16_t idx = 0; idx < query; idx++) {
        this->txt.llg(":",  "#", String(idx + 1), " ::");
        this->txt.llg("hostname", String(MDNS.hostname(idx)));
        this->txt.llg("address", MDNS.IP(idx).toString());
        this->txt.llg("port", String(MDNS.port(idx)));
    }
}

void Cover::loop(void) {
    if (this->repeat <= 1) {
        if ((millis() - this->replay) > COVER_REPLAY) {
            this->repeat = COVER_REPEAT; return;
        }
        return;
    }
    if (this->check(true)) { this->repeat = COVER_REPEAT; return; }
    this->replay = millis();
    this->repeat--; this->apply();
}

uint8_t Cover::cmd_dialup(String _) {
    this->repeat = 0; return (this->apply() ? 0 : 1);
}
uint8_t Cover::cmd_hangup(String _) {
    this->repeat = 0; return (this->hangup() ? 0 : 1);
}
uint8_t Cover::cmd_lookup(String _) { this->lookup(); return 0; }
uint8_t Cover::cmd_status(String _) { this->status(); return 0; }
