#ifndef _light_h_
#define _light_h_

#include <_init.h>
#include "etc/curve.h"

#define LIGHT_LED_RR    D3
#define LIGHT_LED_GG    D2
#define LIGHT_LED_BB    D1
#define LIGHT_PWM_FULL  0
#define LIGHT_PWM_NULL  1024


void light_setup(void);

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

void light_loop(void);

#endif
