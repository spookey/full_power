#include "serve.hpp"

#define _INCL_STRING(...)   #__VA_ARGS__

const String SERVE_BASIC = String(
#include "inc/_basic.html"
);
const String SERVE_FUNCT = String(
#include "inc/_funct.js"
);
const String SERVE_STYLE = String(
#include "inc/_style.css"
);
const String SERVE_PLUGS = String(
#include "inc/_plugs.html"
);

#undef _INCL_STRING

Serve::Serve(Cable& txt, Shell& exe, Cover& net, Light& led, Power& ray)
: txt(txt), exe(exe), net(net), led(led), ray(ray) {
    this->webserver->addHandler(new Index(*this));
}

Serve::Json::Json(Cable& txt, bool verbose)
: txt(txt), verbose(verbose) {}

Serve::Index::Index(Serve& srv)
: srv(srv) {}

void Serve::setup(void) {
    this->webserver->begin();
    this->exe.add(this, &Serve::cmd_restart, "restart", "restart system");
    this->exe.add(this, &Serve::cmd_stats, "stats", "system statistics");
}
void Serve::loop(void) { this->webserver->handleClient(); }

String Serve::Json::show(void) {
    return this->txt.join("{", this->store, "}");
}
void Serve::Json::add(String key, String val, bool raw) {
    String res = (this->store.length() ? ", " : "");
    res = this->txt.join(res, "\"", key, "\": ");
    res = this->txt.join(res, (raw ? val : this->txt.join("\"", val, "\"")));
    this->store = this->txt.join(this->store, res);
    if (this->verbose) { this->txt.llg(key, val); }
}

String Serve::index(void) {
    String plugs, pltpl;
    for (uint8_t idx = 0; idx < POWER_SWITCH; idx++) {
        pltpl = SERVE_PLUGS;
        pltpl.replace("__plugname__", this->ray.name(idx));
        plugs = this->txt.join(plugs, pltpl);
    }
    String resp = SERVE_BASIC;
    resp.replace("__style__", SERVE_STYLE);
    resp.replace("__funct__", SERVE_FUNCT);
    resp.replace("__plugs__", plugs);
    resp.replace("__hostname__", this->net.get_hostname());
    resp.replace("__update__", String(SERVE_UPDATE));
    resp.replace("__status__", this->stats());
    return resp;
}
String Serve::light(String text, bool fade) {
    Json resp = Json(this->txt);
    text.replace("%23", "#");
    if (fade) {
        (text.length() ? this->led.fade(text) : this->led.fade());
    } else {
        (text.length() ? this->led.flash(text) : this->led.flash());
    }
    resp.add("action", (fade ? "fade" : "flash"));
    resp.add("color", text);
    resp.add("status", "ok");
    return resp.show();
}
String Serve::power(String text, bool full) {
    Json resp = Json(this->txt);
    String addr = this->ray.look(text);
    this->ray.full((!addr.length() ? text : addr), full);
    resp.add("action", (full ? "full" : "null"));
    resp.add("address", addr);
    resp.add("status", "ok");
    return resp.show();
}
String Serve::stats(void) {
    Json resp = Json(this->txt, true);
    resp.add("color", this->led.get_color());
    resp.add("dialups", this->net.get_dialups());
    resp.add("hangups", this->net.get_hangups());
    resp.add("launched", this->exe.get_launched());
    resp.add("requests", this->requests);
    resp.add("channel", this->net.get_channel());
    resp.add("signal", this->net.get_signal());
    resp.add("uptime", this->txt.get_uptime());
    return resp.show();
}


bool Serve::Index::canHandle(HTTPMethod meth, String req) {
    if (meth != HTTP_GET) { return false; } req.trim();
    if (req == this->_index || req == this->_stats) { return true; }
    if (req.startsWith(this->_light)) {
        return (
            req.startsWith(this->srv.txt.join(this->_light, "fade/")) ||
            req.startsWith(this->srv.txt.join(this->_light, "flash/"))
        );
    }
    if (req.startsWith(this->_power)) {
        return (
            req.startsWith(this->srv.txt.join(this->_power, "full/")) ||
            req.startsWith(this->srv.txt.join(this->_power, "null/"))
        );
    }
    return false;
}
bool Serve::Index::handle(ESP8266WebServer &webserver, HTTPMethod meth, String req) {
    this->srv.requests++; req.trim();
    this->srv.txt.log("serve", req);
    this->srv.txt.llg("#", String(this->srv.requests));

    if (req == this->_stats) {
        webserver.send(200, "application/json", this->srv.stats()); return true;
    }
    if (req == this->_index) {
        webserver.send(200, "text/html", this->srv.index()); return true;
    }
    if (req.startsWith(this->_light)) {
        String part = req.substring(this->_light.length(), req.length());
        const bool fade = part.startsWith("fade");
        String text = part.substring((fade ?  5 : 6), part.length());
        webserver.send(200, "application/json", this->srv.light(text, fade));
        return true;
    }
    if (req.startsWith(this->_power)) {
        String part = req.substring(this->_power.length(), req.length());
        const bool full = part.startsWith("full");
        String text = part.substring(5, part.length());
        if (text.length()) {
            webserver.send(200, "application/json", this->srv.power(text, full));
            return true;
        }
    }
    return false;
}

uint8_t Serve::cmd_stats(String text) {
    this->txt.log("serve", "stats");
    if (text.length()) {
        this->txt.llg("VCC", String(ESP.getVcc()));
        this->txt.llg("MHz", String(ESP.getCpuFreqMHz()));
        this->txt.llg("chip id", String(ESP.getChipId()));
        this->txt.llg("flash id", String(ESP.getFlashChipId()));
        this->txt.llg("core version", ESP.getCoreVersion());
        this->txt.llg("boot version", String(ESP.getBootVersion()));
        this->txt.llg("free heap", String(ESP.getFreeHeap()));
        this->txt.llg("free sketch", String(ESP.getFreeSketchSpace()));
        this->txt.llg("sketch size", String(ESP.getSketchSize()));
        this->txt.llg("reset raeson", ESP.getResetReason());
    }
    this->stats();
    return 0;
}
