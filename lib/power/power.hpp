#ifndef _power_hpp_
#define _power_hpp_

#include <_init.hpp>
#include <cable.hpp>
#include <shell.hpp>
#include <flash.hpp>

class Power {
public:
    Power(Cable& txt, Shell& exe, Flash& ini);
    void setup(void);

protected:
    Cable& txt;
    Shell& exe;
    Flash& ini;

private:
    struct FullPower {
        const uint8_t hi;
        const uint8_t lo;
        const uint16_t sndlen;

        FullPower(uint8_t hi, uint8_t lo, uint16_t sndlen);
        void burst(uint8_t pin);
    };

    const uint8_t pin = POWER_TX_PIN;
    const uint8_t repeat = POWER_REPEAT;
    FullPower _one = FullPower(3,  1, 350);
    FullPower _syn = FullPower(1, 31, 350);
    FullPower _zer = FullPower(1,  3, 350);

    void send(unsigned long code, int16_t length);
    unsigned long code(const char* trip);

    String key(uint8_t num);
    String address(uint8_t num, bool brief=false);
    void list(void);

    uint8_t cmd_full(String text);
    uint8_t cmd_null(String text);
    uint8_t cmd_sock(String _);

public:
    String name(uint8_t num);
    String look(String name);

    void full(String address, bool power);
};

#endif
