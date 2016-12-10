#ifndef _cable_h_
#define _cable_h_

#include <_init.h>

class Cable {
public:
    Cable(unsigned long baud, SerialConfig conf);
    void setup(void);

protected:
    unsigned long baud;
    SerialConfig conf;

public:
    char collect(void);
    void raw(char data);
    void raw(int data);
    void text(String text, bool nwl=false);

    void log(String name, String text);
    void llg(String name, String text);

    String fill(uint8_t wdt=8, char chr=' ');
    String join(String tx_a, String tx_b);
    String pad(String text, bool pre=true, uint8_t wdt=8,
            char fchr=' ', char schr='.');

    void sos(String reason, bool forever=true, unsigned long wait=2048);

public:
    String join(String tx_a, String tx_b, String tx_c) {
        return this->join(this->join(tx_a, tx_b), tx_c);
    }
    String join(String tx_a, String tx_b, String tx_c, String tx_d) {
        return this->join(this->join(tx_a, tx_b, tx_c), tx_d);
    }

    void log(String name, String tx_a, String tx_b) {
        this->log(name, this->join(tx_a, tx_b));
    }
    void log(String name, String tx_a, String tx_b, String tx_c) {
        this->log(name, this->join(tx_a, tx_b, tx_c));
    }
    void log(String name, String tx_a, String tx_b, String tx_c, String tx_d) {
        this->log(name, this->join(tx_a, tx_b, tx_c, tx_d));
    }

    void llg(String name, String tx_a, String tx_b) {
        this->llg(name, this->join(tx_a, tx_b));
    }
    void llg(String name, String tx_a, String tx_b, String tx_c) {
        this->llg(name, this->join(tx_a, tx_b, tx_c));
    }
    void llg(String name, String tx_a, String tx_b, String tx_c, String tx_d) {
        this->llg(name, this->join(tx_a, tx_b, tx_c, tx_d));
    }
};

#endif
