#include "main.h"

Cable txt = Cable(CABLE_BAUDRT, CABLE_CONFIG);
Shell exe = Shell(txt);
Light led = Light(txt, exe, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);
Store ini = Store(txt, exe, STORE_CONFIG);
Cover net = Cover(txt, exe, ini);
Power pwr = Power(txt, exe, ini, POWER_TX_PIN);

struct Stuff {
    uint8_t uptime(String text) {
        txt.log("stuff", "uptime");
        txt.llg("millis", String(millis()));
        txt.llg("micros", String(micros()));
        return 0;
    }
    uint8_t echo(String text) {
        if (!text.length()) { return 23; }
        txt.text(txt.join("\n", text), true);
        return 0;
    }
    Stuff(Shell& exe) {
        exe.add(this, &Stuff::echo, "echo", "repeat what you just said");
        exe.add(this, &Stuff::uptime, "uptime", "show uptime");
    }
};
Stuff com = Stuff(exe);

void setup(void) {
    txt.setup();
    led.setup();
    ini.setup();
    net.setup();
    pwr.setup();
}

void loop(void) {
    exe.loop();
    net.loop();
}
