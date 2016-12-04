#include "light.h"

void light_setup(void) {
    Serial.begin(115200); Serial.println("...");
    pinMode(LIGHT_LED_RR, OUTPUT);
    pinMode(LIGHT_LED_GG, OUTPUT);
    pinMode(LIGHT_LED_BB, OUTPUT);
    set_rgb(LIGHT_PWM_NULL);
    Serial.println("light_setup done");
}

void set_led(uint8_t pin, uint16_t val) { analogWrite(pin, val); }
void set_r__(uint16_t val) { set_led(LIGHT_LED_RR, val); }
void set__g_(uint16_t val) { set_led(LIGHT_LED_GG, val); }
void set___b(uint16_t val) { set_led(LIGHT_LED_BB, val); }
void set_rg_(uint16_t val) { set_r__(val); set__g_(val); }
void set_r_b(uint16_t val) { set_r__(val); set___b(val); }
void set__gb(uint16_t val) { set__g_(val); set___b(val); }
void set_rgb(uint16_t val) { set_r__(val); set__gb(val); }


void blink(uint8_t times, void (*func)(uint16_t)) {
    for (uint8_t i = 1; i <= times; i++) {
        Serial.println("blink " + String(i) + " full");
        func(LIGHT_PWM_FULL); delay(512);
        Serial.println("blink " + String(i) + " null");
        func(LIGHT_PWM_NULL); delay(512);
    }
    set_rgb(LIGHT_PWM_NULL); delay(512);
    Serial.println("blink done");
}

void fade_linear(void (*func)(uint16_t)) {
    Serial.println("fade_linear full");
    for (uint16_t val = LIGHT_PWM_NULL; val > LIGHT_PWM_FULL; val--) {
        func(val); delay(1);
    }
    Serial.println("fade_linear null");
    for (uint16_t val = LIGHT_PWM_FULL; val < LIGHT_PWM_NULL; val++) {
        func(val); delay(1);
    }
    set_rgb(LIGHT_PWM_NULL); delay(512);
    Serial.println("fade_linear done");
}

void fade_curve(void (*func)(uint16_t)) {
    Serial.println("fade_curve full");
    for (uint16_t val = 1024; val > 0; val--) {
        func(curve[val]); delay(1);
    }
    Serial.println("fade_curve null");
    for (uint16_t val = 0; val < 1024; val++) {
        func(curve[val]); delay(1);
    }
    set_rgb(LIGHT_PWM_NULL); delay(512);
    Serial.println("fade_curve done");
}

void demo(void (*func)(uint16_t)) {
    // blink(2, func);
    fade_linear(func); delay(512);
    // blink(1, func);
    fade_curve(func); delay(512);
}

void light_loop(void) {
    demo(set_r__);
    demo(set__g_);
    demo(set___b);
    demo(set_rg_);
    demo(set_r_b);
    demo(set__gb);
    demo(set_rgb);
    Serial.println("light_loop done");
}
