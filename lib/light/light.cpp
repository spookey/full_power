#include "light.h"

Light::Light(Cable& txt, Shell& exe, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b)
: txt(txt), exe(exe), glare(*this, pin_r, pin_g, pin_b), color() {}

Light::Color::Color(uint8_t rr, uint8_t gg, uint8_t bb)
: rr(rr), gg(gg), bb(bb) {}
Light::Color::Color()
: rr(0x00), gg(0x00), bb(0x00) {}

Light::Glare::Glare(Light& led, uint8_t rr, uint8_t gg, uint8_t bb)
: led(led), rr(rr), gg(gg), bb(bb) { this->generate(); }

uint16_t Light::Glare::luminance(uint8_t val) {
    float value = ((val / ((float) 0xff)) * 100.0);
    if (value <= 8) { value = (value / 902.3); }
    else {
        value = ((value + 16.0) / 116.0);
        value = (value * value * value);
    }
    return (uint16_t) roundf(value * ((float) 0x03ff));
}

void Light::Glare::generate(void) {
    uint16_t val;
    uint8_t idx = 0x00;
    do {
        this->chroma[0xff - idx] = this->luminance(idx);
    } while (idx++ != 0xff);
}

void Light::Glare::set(Color col) {
    analogWrite(this->rr, this->chroma[col.rr]);
    analogWrite(this->gg, this->chroma[col.gg]);
    analogWrite(this->bb, this->chroma[col.bb]);
    this->led.color = col;
}

void Light::setup(void) {
    pinMode(this->glare.rr, OUTPUT);
    pinMode(this->glare.gg, OUTPUT);
    pinMode(this->glare.bb, OUTPUT);
    this->glare.set(Color(0, 0, 0)); this->flash(128, 2); this->fade(8);
    this->exe.add(this, &Light::cmd_flash, "flash", "flash light in color");
    this->exe.add(this, &Light::cmd_fade, "fade", "fade light to color");
}

String Light::Color::show(void) {
    static char res[8];
    sprintf(res, "#%02x%02x%02x", this->rr, this->gg, this->bb);
    return String(res);
}
Light::Color Light::parse(unsigned long value) {
    return Color(
        ((value & 0xff0000) >> 0x10),
        ((value & 0x00ff00) >> 0x08),
        ((value & 0x0000ff) >> 0x00)
    );
}
Light::Color Light::parse(String text) {
    uint8_t base = 10;
    if (text.startsWith("#")) {
        base = 16;
        text = text.substring(1, text.length());
    } else if (text.startsWith("0x") || text.startsWith("0X")) {
        base = 16;
    }
    const char* value = text.c_str();
    return this->parse(strtoul(value, 0, base));
}
Light::Color Light::some(void) { return this->parse(RANDOM_REG32); }

void Light::fade(Color col, unsigned long pause) {
    this->txt.log("light", "fade");
    this->txt.llg("from", this->color.show());
    this->txt.llg("to", col.show());
    this->txt.llg("pause", String(pause));

    Color tmp = this->color;
    uint8_t res = 0;
    while (res != 7) {
        if (tmp.rr < col.rr) { tmp.rr += 1; } else
        if (tmp.rr > col.rr) { tmp.rr -= 1; } else { res |= 1; }
        if (tmp.gg < col.gg) { tmp.gg += 1; } else
        if (tmp.gg > col.gg) { tmp.gg -= 1; } else { res |= 2; }
        if (tmp.bb < col.bb) { tmp.bb += 1; } else
        if (tmp.bb > col.bb) { tmp.bb -= 1; } else { res |= 4; }
        this->glare.set(tmp); delay(pause);
    }
}
void Light::flash(Color col, uint8_t num, unsigned long pause) {
    this->txt.log("light", "flash");
    this->txt.llg("from", this->color.show());
    this->txt.llg("in", col.show());
    this->txt.llg("times", String(num));
    this->txt.llg("pause", String(pause));

    Color tmp = this->color;
    for (; num; num--) {
        this->glare.set(col); delay(pause);
        this->glare.set(tmp); delay(pause);
    }
}

uint8_t Light::cmd_fade(String text) {
    if (!text.length()) { this->fade(); } else { this->fade(text); }
    return 0;
}
uint8_t Light::cmd_flash(String text) {
    if (!text.length()) { this->flash(2); } else { this->flash(text, 2); }
    return 0;
}
