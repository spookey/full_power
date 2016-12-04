#ifndef _compare_light_h_
#define _compare_light_h_

#include "light.h"
#include "lookup_light.h"

static uint16_t gen_curve[1025] = {0};

uint16_t luminance_step(uint16_t val, uint16_t inputSize, uint16_t outputSize);
void setup_curve(uint16_t* the_curve);

void compare_light_setup(void);
void compare_light_loop(void);

#endif
