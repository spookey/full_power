#include "main.h"

Cable txt = Cable(CABLE_BAUD, CABLE_CONF);
Light led = Light(txt, LIGHT_LED_RR, LIGHT_LED_GG, LIGHT_LED_BB);
Shell cmd = Shell(txt);

void setup(void) {
    txt.setup();
    led.setup();
    cmd.setup();

    cmd.append("error", "return some error", [](String _) -> int8_t {
        return -0x80;
    });
    cmd.append("echo", "what you just said", [](String text) -> int8_t {
        txt.text(txt.join("\n", text), true); return 0x00;
    });
    cmd.append("rnd", "random 32 bit number", [](String _) -> int8_t {
        int32_t rnd = RANDOM_REG32;
        txt.log("Light", "random");
        txt.llg("dec", String(rnd, DEC));
        txt.llg("hex", String(rnd, HEX));
        txt.llg("bin", String(rnd, BIN));
        return 0x00;
    });
}

void loop(void) {
    txt.loop();
    led.loop();
    cmd.loop();
}
