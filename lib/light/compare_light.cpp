#include "compare_light.h"

uint16_t luminance_step(uint8_t val) {
    float value = ((val / ((float) 0xff)) * 100.0);
    if (value <= 8) {
        value = (value / 902.3);
    } else {
        value = ((value + 16.0) / 116.0);
        value = (value * value * value);
    }
    return (uint16_t) roundf(value * ((float) 0x03ff));
}

void setup_curve(uint16_t* the_curve) {
    uint8_t num = 0x00; do {
        the_curve[0xff - num] = luminance_step(num);
    } while (num++ != 0xff);
}

void compare_light_setup(void) {
    lookup_light_setup();
    setup_curve(gen_curve);
    Serial.println("compare_light_setup done");
}

void compare_light_loop(void) {
    uint8_t wrong = 0;
    uint8_t num = 0x00; do {
        char line[64];
        bool yeah = (lookup_curve[num] == gen_curve[num]);
        if (!yeah) { wrong++; }
        sprintf(
            line, "%2x | %i || %04d = %04d",
            num, yeah, lookup_curve[num], gen_curve[num]
        );
        Serial.println(line);
        delay(64);
    } while (num++ != 0xff);

    if (wrong == 0) { lookup_light_loop(); }
    else { Serial.println("wrong: " + String(wrong)); }
    delay(2048);
    Serial.println("compare_light_loop done");
}
