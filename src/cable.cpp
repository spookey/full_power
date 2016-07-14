#include "cable.h"

Cable::Cable(unsigned long baud, SerialConfig conf) {
    Serial.begin(baud, conf);
    delay(1);
    while (!Serial) { delay(0.01); }
}

void Cable::llog(String one) {
    Serial.print(one);
}

void Cable::log(String one) {
    Serial.println(one);
    Serial.flush();
}
void Cable::log(String one, String two) {
    llog(one);
    log(two);
}
void Cable::log(String one, String two, String three) {
    llog(one);
    log(two, three);
}
void Cable::log(String one, String two, String three, String four) {
    llog(one);
    log(two, three, four);
}

String Cable::color(uint32_t col) {
    char res[8]; sprintf(res, "0x%06x", col);
    return String(res);
}
String Cable::color(uint8_t col_r, uint8_t col_g, uint8_t col_b) {
    char res[8]; sprintf(res, "0x%02x%02x%02x", col_r, col_g, col_b);
    return String(res);
}
