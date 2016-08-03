#include "cover.h"

Cover::Cover(const Cable& msg, String ssid, String secret, String identifier)
: msg(msg), ssid(ssid), secret(secret), identifier(identifier) {
    WiFi.mode(WIFI_STA);
}

void Cover::dialup(void) {
    if (WiFi.isConnected()) {
        return;
    }
    this->msg.log("Cover\tdialup     '", ssid, "'");

    WiFi.hostname(identifier);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid.c_str(), secret.c_str());

    uint8_t wait = 0;
    while (!WiFi.isConnected()) {
        if (++wait == 0) {
            this->msg.llog();
            this->msg.log("Cover\tdialling up! (", String(WiFi.status()), ")");
        }
        this->msg.llog(":"); delay(5);
    };
    this->msg.llog();
    this->dialupCount++;

    if (MDNS.begin(identifier.c_str(), WiFi.localIP())) {
        MDNS.addService("http", "tcp", SERVE_NET_PORT);
        this->msg.log("Cover\tmDNS registered");
    }
    this->info();
}

void Cover::info(void) {
    if (!WiFi.isConnected()) {
        this->msg.log("Cover\tnot connected! - ", String(WiFi.status()));
        this->dialup();
    }

    this->msg.log("Cover\tconnected  '", WiFi.SSID(), "'");
    this->msg.log("     \tbssid      ", WiFi.BSSIDstr());
    this->msg.log("     \tmac        ", WiFi.macAddress());
    this->msg.log("     \tchannel    #", String(WiFi.channel()));
    this->msg.log("     \tsignal     ", this->getSignal());
    this->msg.log("     \t'", this->getHostname(), "'");
    this->msg.log("     \taddress    ", WiFi.localIP().toString());
    this->msg.log("     \tsubnet     ", WiFi.subnetMask().toString());
    this->msg.log("     \tgateway    ", WiFi.gatewayIP().toString());
    this->msg.log("     \tdns        ", WiFi.dnsIP().toString());
    this->msg.log("     \tuptime     ", this->msg.uptime());
}
