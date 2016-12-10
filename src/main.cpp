#include "main.h"

Cable txt = Cable(CABLE_BAUDRT, CABLE_CONFIG);
Shell exe = Shell(txt);
Store ini = Store(txt, exe, STORE_CONFIG);
Light led = Light(txt, exe, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);
Cover net = Cover(txt, exe, ini);

struct Stuff {
    uint8_t uptime(String text) {
        txt.log("stuff", "uptime");
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
    ini.setup();
    net.setup();
    led.setup();
}

void loop(void) {
    exe.loop();
    net.loop();
}
