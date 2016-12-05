#ifndef _lookup_light_h_
#define _lookup_light_h_

#include "light.h"

void lookup_light_setup(void);
uint16_t blow(uint8_t val);

void set_led(uint8_t pin, uint16_t val);
void set___b(uint16_t val);
void set__g_(uint16_t val);
void set__gb(uint16_t val);
void set_r__(uint16_t val);
void set_r_b(uint16_t val);
void set_rg_(uint16_t val);
void set_rgb(uint16_t val);

void blink(uint8_t times, void (*func)(uint16_t));
void fade_linear(void (*func)(uint16_t));
void fade_curve(void (*func)(uint16_t));

void demo(void (*func)(uint16_t));

void lookup_light_loop(void);

#endif
