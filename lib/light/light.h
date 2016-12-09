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

    struct Color {
        uint8_t rr; uint8_t gg; uint8_t bb;

        Color(void);
        Color(uint8_t rr, uint8_t gg, uint8_t bb);
        String show(void);
    };

    Color parse(unsigned long value);
    Color parse(String text);
    Color some(void);

    struct Glare {
        Light& led;
        uint8_t rr; uint8_t gg; uint8_t bb;
        uint16_t chroma[1 + 0xff];

        Glare(Light& led, uint8_t rr, uint8_t gg, uint8_t bb);
        uint16_t luminance(uint8_t val);
        void generate(void);
        void set(Color col);
    };
    Color color;
    Glare glare;

    void fade(Color col, unsigned long pause=16);
    void flash(Color col, uint8_t num=2, unsigned long pause=128);

public:
    void fade(unsigned long pause=16) {
        this->fade(this->some(), pause);
    }
    void fade(String text, unsigned long pause=16) {
        this->fade(this->parse(text), pause);
    }
    void flash(uint8_t num=2, unsigned long pause=128) {
        this->flash(this->some(), num, pause);
    }
    void flash(String text, uint8_t num=2, unsigned long pause=128) {
        this->flash(this->parse(text), num, pause);
    }

    uint8_t cmd_fade(String text) {
        if (text.length() > 0) { this->fade(text); }
        else { this->fade(); }
        return 0;
    }
    uint8_t cmd_flash(String text) {
        if (text.length() > 0) { this->flash(text, 2); }
        else { this->flash(2); }
        return 0;
    }
};

#endif
