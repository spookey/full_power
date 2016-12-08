#include "main.h"

Cable txt = Cable(CABLE_BAUDRT, CABLE_CONFIG);
Light led = Light(txt, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);

void setup(void) {
    txt.setup();
    led.setup();
    txt.log("_main", "setup done");
}

void loop(void) {
    txt.loop();
    led.loop();
    led.fade(); delay(1024);
}
