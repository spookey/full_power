#include "compare_light.h"

uint16_t luminance_step(uint16_t val, uint16_t inputSize, uint16_t outputSize) {
    float value = ((val / ((float) inputSize)) * 100.0);
    if (value <= 8) {
        value = (value / 902.3);
    } else {
        value = ((value + 16.0) / 116.0);
        value = (value * value * value);
    }
    return (uint16_t) roundf(value * ((float) outputSize));
}

void setup_curve(uint16_t* the_curve) {
    for (uint16_t num = LIGHT_PWM_FULL; num <= LIGHT_PWM_NULL; num++) {
        the_curve[num] = luminance_step(num, 1024, 1024);
    };
}

void compare_light_setup(void) {
    Serial.begin(115200); Serial.println("...");
    pinMode(LIGHT_LED_RR, OUTPUT); analogWrite(LIGHT_LED_RR, LIGHT_PWM_NULL);
    pinMode(LIGHT_LED_GG, OUTPUT); analogWrite(LIGHT_LED_GG, LIGHT_PWM_NULL);
    pinMode(LIGHT_LED_BB, OUTPUT); analogWrite(LIGHT_LED_BB, LIGHT_PWM_NULL);

    setup_curve(gen_curve);
    Serial.println("compare_light_setup done");
}

void compare_light_loop(void) {
    uint16_t wrong = 0;
    for (uint16_t num = 0; num <= 1024; num++) {
        char line[64];
        bool yeah = (lookup_curve[num] == gen_curve[num]);
        if (!yeah) { wrong++; }
        sprintf(
            line, "%04d | %i || %04d = %04d",
            num, yeah, lookup_curve[num], gen_curve[num]
        );
        Serial.println(line);
        delay(64);
    }
    if (wrong == 0) { blink(3, set_rgb); }
    else { Serial.println("wrong: " + String(wrong)); }
    delay(2048);
    Serial.println("compare_light_loop done");
}
