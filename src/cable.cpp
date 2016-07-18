#include "cable.h"

Cable::Cable(unsigned long baud, SerialConfig conf) {
    Serial.begin(baud, conf);
    delay(1);
    while (!Serial) { delay(0.01); }
}

String Cable::_join(String first, String second) {
    first += second;
    return first;
}


void Cable::llog(String one) {
    Serial.print(one);
}

void Cable::log(String one) {
    Serial.println(one);
    Serial.flush();
}


String Cable::uptime(void) {
    unsigned long sec = millis() / 1000;
    unsigned long min = sec / 60;
    unsigned long hrs = min / 60;
    unsigned long day = hrs / 24;
    sec = sec - (min * 60);
    min = min - (hrs * 60);
    hrs = hrs - (day * 24);
    static char res[16];
    (day >= 1) ? sprintf(
        res, "%dd %02d:%02d:%02d", day, hrs, min, sec
    ) : sprintf(
        res, "%02d:%02d:%02d", hrs, min, sec
    );
    return String(res);
}

String Cable::color(uint32_t col) {
    static char res[8]; sprintf(res, "#%06x", col);
    return String(res);
}
String Cable::color(uint8_t col_r, uint8_t col_g, uint8_t col_b) {
    static char res[8]; sprintf(res, "#%02x%02x%02x", col_r, col_g, col_b);
    return String(res);
}
