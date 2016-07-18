#ifndef __power_h__
#define __power_h__

#include "_init.h"
#include "cable.h"

class Power {
public:
    Power(const Cable& msg, uint8_t spin, uint8_t repeat = 10);
    void full(const char* address, bool power);

private:
    Cable msg;
    uint8_t spin;
    uint8_t repeat;

    uint16_t length;
    struct FullPower { uint8_t hi; uint8_t lo; };
    FullPower _one;
    FullPower _syn;
    FullPower _zer;

    void _send(const char* word);
    void _burst(FullPower power);
};

#endif
