#include "main.h"

Cable txt = Cable(CABLE_BAUD, CABLE_CONF);

int8_t the_echo(String text) {
    txt.text(txt.join("\n", text), true); return 0;
}
int8_t the_error(String text) { return -0x80; }

void setup(void) {
    txt.setup();
    txt.append("error", the_error);
    txt.append("echo", the_echo);
}

void loop(void) {
    txt.loop();
}
