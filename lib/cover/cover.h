#ifndef _cover_h_
#define _cover_h_

#include <ESP8266WiFi.h>
#include <ESP8266WiFiType.h>
#include <ESP8266mDNS.h>

#include <_init.h>
#include <cable.h>
#include <shell.h>
#include <store.h>

class Cover {
public:
    Cover(Cable& txt, Shell& exe, Store& ini);
    void setup(void);
    void loop(void);

protected:
    Cable& txt;
    Shell& exe;
    Store& ini;
    uint16_t dialups = 0;
    uint16_t hangups = 0;

private:
    uint8_t repeat = COVER_REPEAT;
    unsigned long replay = millis();
    bool hangup(void);
    bool check(bool quick=false);
    bool apply(void);
    void status(void);
    void lookup(void);
    uint8_t cmd_dialup(String _);
    uint8_t cmd_hangup(String _);
    uint8_t cmd_lookup(String _);
    uint8_t cmd_status(String _);
};

#endif
