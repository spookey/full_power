#include "serve.h"

#define STRING(...)   #__VA_ARGS__

const String TEMPLATE_OUTER = String(
#include "template_outer.html"
);
const String TEMPLATE_INNER = String(
#include "template_inner.html"
);


Serve::Serve(
    const Cable& msg, const Cover& net, const Light& led, const Power& pwr
)
: msg(msg), net(net), led(led), pwr(pwr),
server(ESP8266WebServer(SERVE_NET_PORT))
{
    this->server.on("/", [this](){
        this->requestCount++;
        this->_action();
        this->server.send(200, "text/html", this->_load(TEMPLATE_INNER));
    });
    this->server.onNotFound([this](){
        this->requestCount++;
        this->server.send(404, "text/html", this->_load(""));
    });
}

void Serve::setup(void) {
    this->net.dialup();
    this->led.initTune();
    this->led.fade(this->led.someColor());
    this->server.begin();
}

void Serve::loop(void) {
    server.handleClient();
}

void Serve::_action(void) {
    if (!this->server.args()) { return; }
    String color = this->server.arg("color");
    String light = this->server.arg("light");
    String power = this->server.arg("power");
    String state = this->server.arg("state");

    if (power.length() && state.length()) {
        if (
            state.equalsIgnoreCase("on") ||
            state.equalsIgnoreCase("true") ||
            state.equalsIgnoreCase("FULL") ||
            state.equalsIgnoreCase("✓")
        ) {
            this->pwr.full(power.c_str(), true);
        } else if (
            state.equalsIgnoreCase("off") ||
            state.equalsIgnoreCase("false") ||
            state.equalsIgnoreCase("NULL") ||
            state.equalsIgnoreCase("✗")
        ) {
            this->pwr.full(power.c_str(), false);
        } else {
            this->msg.log("Serve\tweird power command!");
            this->msg.log("     \tplug       '", power, "'");
            this->msg.log("     \tstate      '", state, "'");
        }
    }

    if (light.length() && color.length()) {
        const char* cc = color.c_str();
        uint32_t col = (uint32_t) strtoul(&cc[1], NULL, 16);
        this->msg.log("#", String(col));
        if (
            light.equalsIgnoreCase("fade") ||
            light.equalsIgnoreCase("✎")
        ) {
            this->led.fade(col);
        } else if (
            light.equalsIgnoreCase("flash") ||
            light.equalsIgnoreCase("★")
        ) {
            this->led.flashTwice(col);
        } else {
            this->msg.log("Serve\tweird light command!");
            this->msg.log("     \tlight      '", light, "'");
            this->msg.log("     \tcolor      '", color, "'");
        }
    }
}

String Serve::_load(String inner) {
    String response = TEMPLATE_OUTER;
    inner.replace("__color__", this->msg.color(this->led.someColor()));
    inner.replace("__family__", SERVE_FAMILY);
    response.replace("__color__", this->led.currentColor());
    response.replace("__info__", this->msg.uptime());
    response.replace("__inner__", inner);
    response.replace("__title__", this->net.getHostname());
    return response;
}