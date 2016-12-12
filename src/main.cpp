#include "main.h"

Cable txt = Cable(CABLE_BAUDRT, CABLE_CONFIG);
Shell exe = Shell(txt);
Store ini = Store(txt, exe, STORE_CONFIG);
Cover net = Cover(txt, exe, ini);
Light led = Light(txt, exe, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);
Power ray = Power(txt, exe, ini, POWER_TX_PIN);
Serve web = Serve(txt, led, ray);

struct Stuff {
    uint8_t echo(String text) {
        if (!text.length()) { return 23; }
        txt.text(txt.join("\n", text), true);
        return 0;
    }
    uint8_t info(String _) {
        txt.log("stuff", "info");
        txt.llg("VCC", String(ESP.getVcc()));
        txt.llg("MHz", String(ESP.getCpuFreqMHz()));
        txt.llg("uptime", String(millis()));
        txt.llg("chip id", String(ESP.getChipId()));
        txt.llg("flash id", String(ESP.getFlashChipId()));
        txt.llg("core version", ESP.getCoreVersion());
        txt.llg("boot version", String(ESP.getBootVersion()));
        txt.llg("free heap", String(ESP.getFreeHeap()));
        txt.llg("free sketch", String(ESP.getFreeSketchSpace()));
        txt.llg("sketch", String(ESP.getSketchSize()));
        txt.llg("reset raeson", ESP.getResetReason());
        return 0;
    }
    Stuff(Shell& exe) {
        exe.add(this, &Stuff::echo, "echo", "repeat what you just said");
        exe.add(this, &Stuff::info, "info", "shows system info");
    }
};
Stuff com = Stuff(exe);

void setup(void) {
    txt.setup();
    ini.setup();
    net.setup();
    led.setup();
    ray.setup();
    web.setup();
}

void loop(void) {
    exe.loop();
    net.loop();
    web.loop();
}
