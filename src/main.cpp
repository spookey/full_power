#include "main.h"

Cable txt = Cable(CABLE_BAUD, CABLE_CONF);
Light led = Light(txt, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);
Shell cmd = Shell(txt);

void setup(void) {
    txt.setup();
    led.setup();
    cmd.setup();

    txt.log("_main", "setup done");
}

void loop(void) {
    txt.loop();
    led.loop();
    cmd.loop();

    led.fade(); delay(1024);
}
