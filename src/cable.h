#ifndef __cable_h__
#define __cable_h__

#include "_init.h"

class Cable {
public:
    Cable(unsigned long baud, SerialConfig conf);

    void llog(String msg);
    void llog() {
        this->llog("\n");
    }

    void log(String msg);
    void log(String one, String two) {
        this->log(this->_join(one, two));
    }
    void log(String one, String two, String three) {
        this->log(one, this->_join(two, three));
    }
    void log(String one, String two, String three, String four) {
        this->log(one, two, this->_join(three, four));
    }

    String uptime(void);

    String color(uint32_t col);
    String color(uint8_t cr, uint8_t cg, uint8_t cb);


private:
    String _join(String first, String second);
};

#endif
