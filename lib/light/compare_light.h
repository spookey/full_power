#ifndef _compare_light_h_
#define _compare_light_h_

#include "light.h"
#include "lookup_light.h"

static uint16_t gen_curve[1 + 0xff];

uint16_t luminance_step(uint8_t val);
void setup_curve(uint16_t* the_curve);

void compare_light_setup(void);
void compare_light_loop(void);

#endif
