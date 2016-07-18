#include "light.h"

Light::Light(const Cable &msg, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b)
: msg(msg), pin_r(pin_r), pin_g(pin_g), pin_b(pin_b) {
    pinMode(pin_r, OUTPUT);
    pinMode(pin_g, OUTPUT);
    pinMode(pin_b, OUTPUT);
    set(0x000000);
}

uint16_t Light::_crunch(uint8_t val) {
    return map(val, 0x00, 0xff, LIGHT_PWM_OFF, LIGHT_PWM_ON);
}
uint8_t Light::_getR(uint32_t col) {
    return (col & 0xff0000) >> 16;
}
uint8_t Light::_getG(uint32_t col) {
    return (col & 0x00ff00) >> 8;
}
uint8_t Light::_getB(uint32_t col) {
    return (col & 0x0000ff);
}
void Light::_write(uint8_t pin, uint16_t col) {
    analogWrite(pin, this->_crunch(col));
}

void Light::_set(uint8_t col_r, uint8_t col_g, uint8_t col_b) {
    this->_write(pin_r, col_r); cur_r = col_r;
    this->_write(pin_g, col_g); cur_g = col_g;
    this->_write(pin_b, col_b); cur_b = col_b;
}

void Light::_fade(
    uint8_t col_r, uint8_t col_g, uint8_t col_b, unsigned long pause
) {
    this->msg.log("Fade\t",
        this->msg.color(cur_r, cur_g, cur_b), " -> ",
        this->msg.color(col_r, col_g, col_b)
    );
    uint8_t new_r = cur_r;
    uint8_t new_g = cur_g;
    uint8_t new_b = cur_b;
    for(;;) {
        if (new_r < col_r) {new_r += 1;} else
        if (new_r > col_r) {new_r -= 1;} else
        if (new_g < col_g) {new_g += 1;} else
        if (new_g > col_g) {new_g -= 1;} else
        if (new_b < col_b) {new_b += 1;} else
        if (new_b > col_b) {new_b -= 1;}
        else {break;}
        this->_set(new_r, new_g, new_b);
        delay(pause);
    };
}

void Light::flash(uint32_t col, unsigned long pause) {
    this->msg.log("Flash\t",
        this->msg.color(cur_r, cur_g, cur_b), " -> ",
        this->msg.color(col)
    );
    uint8_t old_r = cur_r;
    uint8_t old_g = cur_g;
    uint8_t old_b = cur_b;
    this->set(col);
    delay(pause);
    this->_set(old_r, old_g, old_b);
}

void Light::flashTwice(uint32_t col, unsigned long pause) {
    this->flash(col, pause); delay(pause);
    this->flash(col, pause); delay(pause);
}
