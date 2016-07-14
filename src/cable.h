#ifndef __cable_h__
#define __cable_h__

#include "_init.h"

class Cable {
public:
    Cable(unsigned long baud, SerialConfig conf);

    void llog(String msg);
    void llog() {
        llog("\n");
    }

    void log(String msg);
    void log(String one, String two);
    void log(String one, String two, String three);
    void log(String one, String two, String three, String four);

    String color(uint32_t col);
    String color(uint8_t cr, uint8_t cg, uint8_t cb);
};

#endif
