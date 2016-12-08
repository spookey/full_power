#include "main.h"

Cable txt = Cable(CABLE_BAUD, CABLE_CONF);
Light led = Light(txt, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);

struct Status {
    uint8_t uptime(String text) {
        txt.log("_main", "uptime");
        txt.llg("millis", String(millis()));
        txt.llg("text", text);
        return 0;
    }
};

Status sts = Status();
Shell phone = Shell(&sts, &Status::uptime, "uptime");

void setup(void) {
    txt.setup();
    led.setup();
    txt.log("_main", "setup done");
}

void loop(void) {
    txt.loop();
    led.loop();
    phone.call(String(millis()));
    led.fade(); delay(1024);
}
