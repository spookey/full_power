#include "power.h"

Power::Power(const Cable& msg, uint8_t spin, uint8_t repeat)
: msg(msg), spin(spin), repeat(repeat), length(350),
_one(FullPower({ 3, 1 })), _syn(FullPower({ 1, 31 })), _zer(FullPower({ 1, 3 }))
 {
    pinMode(this->spin, OUTPUT);
    digitalWrite(this->spin, LOW);
}

void Power::full(const char* address, bool power) {
    static char res[13];
    int pos = 0;
    for (int i = 0; i < 10; i++) {
        res[pos++] = (address[i] == '0') ? 'F' : '0';
    }
    res[pos++] = power ? '0' : 'F';
    res[pos++] = power ? 'F' : '0';
    res[pos] = '\0';
    this->msg.log("Power\taddress    ", String(address));
    this->msg.log("     \tstate      ", (power ? "FULL" : "NULL"));
    this->_send(res);
}

void Power::_send(const char* word) {
    unsigned long code = 0;
    unsigned int len = 0;
    for (const char* w = word; *w; w++) {
        code <<= 2L;
        switch (*w) {
            case '0': break;
            case 'F': code |= 1L; break;
            case '1': code |= 3L; break;
        }
        len += 2;
    }
    for (uint8_t _ = 0; _ < repeat; _++) {
        for (int l = len-1; l >= 0; l--) {
            if (code & (1L << l)) { this->_burst(this->_one); }
            else { this->_burst(this->_zer); }
        }
        this->_burst(this->_syn);
    }
}

void Power::_burst(FullPower power) {
    digitalWrite(this->spin, HIGH);
    delayMicroseconds(this->length * power.hi);
    digitalWrite(this->spin, LOW);
    delayMicroseconds(this->length * power.lo);
}
