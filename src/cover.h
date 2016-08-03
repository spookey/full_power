#ifndef __cover_h__
#define __cover_h__

#include <ESP8266WiFi.h>
#include <ESP8266WiFiType.h>
#include <ESP8266mDNS.h>

#include "_init.h"
#include "cable.h"

class Cover {
public:
    Cover(const Cable& msg, String ssid, String secret, String identifier);
    void dialup(void);
    void info(void);

    uint32_t getDialups(void) {
        return this->dialupCount;
    }
    String getHostname(void) {
        return WiFi.hostname();
    }
    String getSignal(void) {
        return String(String(WiFi.RSSI()) + " dBm");
    }

private:
    Cable msg;
    String ssid;
    String secret;
    String identifier;
    uint32_t dialupCount = 0;
};

#endif
