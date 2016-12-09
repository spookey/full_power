#include "main.h"

Cable txt = Cable(CABLE_BAUDRT, CABLE_CONFIG);
Shell exe = Shell(txt);
Light led = Light(txt, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);

struct Stuff {
    uint8_t uptime(String text) {
        txt.log("_main", "uptime");
        txt.llg("millis", String(millis()));
        return 0;
    }
    uint8_t echo(String text) {
        if (text.length() > 0) {
            txt.text(txt.join("\n", text), true);
            return 0;
        }
        return 23;
    }
    Stuff(Shell& exe) {
        exe.add(this, &Stuff::echo, "echo", "repeat what you just said");
        exe.add(this, &Stuff::uptime, "uptime", "show uptime in millis");
    }
};
Stuff com = Stuff(exe);

void setup(void) {
    txt.setup();
    exe.setup();
    led.setup();

    exe.add(&led, &Light::cmd_flash, "flash", "flash light in given color");
    exe.add(&led, &Light::cmd_fade, "fade", "flade light to given color");
    txt.log("_main", "setup done");
}

void loop(void) {
    txt.loop();
    exe.loop();
    led.loop();
}
