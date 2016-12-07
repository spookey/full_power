#ifndef _light_h_
#define _light_h_

#include <_init.h>
#include <cable.h>

class Light {
public:
    Light(Cable& txt, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b);
    void setup(void);
    void loop(void);

private:
    Cable& txt;
};

#endif
