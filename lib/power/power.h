#ifndef _power_h_
#define _power_h_

#include <_init.h>
#include <cable.h>
#include <shell.h>
#include <store.h>

class Power {
public:
    Power(Cable& txt, Shell& exe, Store& ini, uint8_t pin);
    void setup(void);

protected:
    Cable& txt;
    Shell& exe;
    Store& ini;

private:
    struct FullPower { uint8_t hi; uint8_t lo; };
    uint8_t pin;
    uint8_t repeat = POWER_REPEAT;
    uint16_t sndlen = 350;
    FullPower _one = FullPower({3, 1});
    FullPower _syn = FullPower({1, 31});
    FullPower _zer = FullPower({1, 3});
    void burst(FullPower power);
    void send(const char* word);
    void full(const char* address, bool power);

    String key(uint8_t num);
    String address(uint8_t num, bool brief=false);
    String look(String name);
    void list(void);

    uint8_t cmd_full(String text);
    uint8_t cmd_null(String text);
    uint8_t cmd_sock(String _);

public:
    String name(uint8_t num);

    void full(String address, bool power) {
        this->full(address.c_str(), power);
    }
    void full(uint8_t num, bool power) {
        this->full(this->address(num).c_str(), power);
    }
};

#endif
