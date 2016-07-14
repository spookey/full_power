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
    uint32_t reconnects(void) {
        return reconnectCount;
    }

private:
    Cable msg;
    String ssid;
    String secret;
    String identifier;
    uint32_t reconnectCount = 0;
};

#endif
