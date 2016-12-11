#include "cable.h"

Cable::Cable(unsigned long baud, SerialConfig conf)
: baud(baud), conf(conf) {}

void Cable::setup(void) {
    Serial.begin(this->baud, this->conf);
    do { delay(0.25); } while (!Serial);
    this->text(this->fill(0xff, '^'), true);
}

char Cable::collect(void) {
    int16_t val = Serial.read();
    if (val < 0) { return _CHAR_IGNORE; }
    return (char) val;
}
void Cable::raw(char data) { Serial.write(data); }
void Cable::raw(int data) { Serial.write(data); }
void Cable::text(String text, bool nwl) {
    (nwl ? Serial.println(text) : Serial.print(text)); Serial.flush();
}

String Cable::join(String tx_a, String tx_b) {
    tx_a.reserve(tx_a.length() + tx_b.length());
    tx_a += tx_b; return tx_a;
}
String Cable::fill(uint8_t wdt, char chr) {
    String res; res.reserve(wdt);
    while(wdt--) { res += chr; }
    return res;
}
String Cable::pad(String text, bool pre, uint8_t wdt, char fchr, char schr) {
    if (wdt <= 2) { return this->fill(wdt, schr); }
    const uint32_t len = text.length();
    if (wdt >= len) {
        String fill = this->fill(wdt - len, fchr);
        return (pre ? this->join(fill, text) : this->join(text, fill));
    }
    return text.substring(0, wdt - 2) + schr + schr;
}

void Cable::log(String name, String text) {
    name.toUpperCase();
    this->text(this->join("\n", this->pad(name, false, 8), " ", text), true);
}
void Cable::llg(String name, String text) {
    this->text(this->join(this->pad(name, true, 16), ": ", text), true);
}

void Cable::sos(String reason, bool forever, unsigned long wait) {
    String dit = this->join(this->fill(255), this->fill(255));
    String dah = this->join(dit, dit, dit, dit);
    while (forever || this->collect() != _CHAR_IGNORE) {
        for (uint8_t idx = 0; idx <= 2; idx++) {
            for (uint8_t _ = 0; _ <= 2; _++) {
                this->text((idx == 1) ? dah : dit, true); delay(512);
            }
            delay(512);
        }
        this->log("cable", "alarm"); this->llg("reason", reason); delay(wait);
    }
}
