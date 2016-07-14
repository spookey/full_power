#include "cover.h"

Cover::Cover(const Cable& msg, String ssid, String secret, String identifier)
: msg(msg), ssid(ssid), secret(secret), identifier(identifier) {
    WiFi.mode(WIFI_STA);
}

void Cover::dialup(void) {
    if (WiFi.isConnected()) {
        return;
    }
    msg.log("Cover\tdialup     '", ssid, "'");

    WiFi.hostname(identifier);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid.c_str(), secret.c_str());

    uint8_t waitcount = 0;
    while (!WiFi.isConnected()) {
        msg.llog(":"); delay(500);
        waitcount++;
        if (waitcount >= 23) {
            msg.llog();
            msg.log("Cover\tstill dialling! - ", String(WiFi.status()));
            waitcount = 0;
        }
    };
    msg.llog();
    reconnectCount++;

    if (!MDNS.begin(identifier.c_str())) {
        msg.log("Cover\tmDNS not possible!");
        return;
    }
    MDNS.addService("http", "tcp", SERVE_NET_PORT);
    info();
}

void Cover::info(void) {
    if (!WiFi.isConnected()) {
        msg.log("Cover\tnot connected! - ", String(WiFi.status()));
        dialup();
    }

    msg.log("Cover\tconnected  '", WiFi.SSID(), "'");
    msg.log("     \tbssid      ", WiFi.BSSIDstr());
    msg.log("     \tmac        ", WiFi.macAddress());
    msg.log("     \tchannel    #", String(WiFi.channel()));
    msg.log("     \tsignal     ", String(WiFi.RSSI()), " dBm");
    msg.log("     \t'", WiFi.hostname(), "'");
    msg.log("     \taddress    ", WiFi.localIP().toString());
    msg.log("     \tsubnet     ", WiFi.subnetMask().toString());
    msg.log("     \tgateway    ", WiFi.gatewayIP().toString());
    msg.log("     \tdns        ", WiFi.dnsIP().toString());
}
