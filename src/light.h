#ifndef __light_h_
#define __light_h_

#include "_init.h"
#include "cable.h"

class Light {
public:
    Light(
        const Cable &msg, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b
    );
    void set(uint32_t col) {
        this->_set(this->_getR(col), this->_getG(col), this->_getB(col));
    }
    void fade(uint32_t col, unsigned long pause = 5) {
        this->_fade(
            this->_getR(col), this->_getG(col), this->_getB(col), pause
        );
    }

    void flash(uint32_t col = 0x000000, unsigned long pause = 100);
    void flashTwice(uint32_t col = 0x000000, unsigned long pause = 100);

    void initTune(void) {
        this->fade(0xffffff, 1); this->flashTwice(0x000000, 20);
        this->fade(0x000000, 1); this->flashTwice(0xffffff, 20);
    }
    uint32_t someColor(void) {
        return uint32_t(random(0x000000, 0xffffff));
    }
    String currentColor(void) {
        return this->msg.color(this->cur_r, this->cur_g, this->cur_b);
    }

private:
    Cable msg;
    uint8_t pin_r;
    uint8_t pin_g;
    uint8_t pin_b;

    uint8_t cur_r;
    uint8_t cur_g;
    uint8_t cur_b;

    uint16_t _crunch(uint8_t val);
    uint8_t _getR(uint32_t col);
    uint8_t _getG(uint32_t col);
    uint8_t _getB(uint32_t col);
    void _write(uint8_t pin, uint16_t col);

    void _set(uint8_t col_r, uint8_t col_g, uint8_t col_b);
    void _fade(
        uint8_t col_r, uint8_t col_g, uint8_t col_b, unsigned long pause
    );
};

#endif
