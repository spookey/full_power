#include "power.hpp"

Power::Power (Cable& txt, Shell& exe, Flash& ini)
: txt(txt), exe(exe), ini(ini) {}

Power::FullPower::FullPower(uint8_t hi, uint8_t lo, uint16_t sndlen)
: hi(hi), lo(lo), sndlen(sndlen) {}

void Power::setup(void) {
    pinMode(this->pin, OUTPUT); digitalWrite(this->pin, LOW);
    this->list();
    this->exe.add(this, &Power::cmd_full, "full", "full power");
    this->exe.add(this, &Power::cmd_null, "null", "null power");
    this->exe.add(this, &Power::cmd_sock, "sock", "list sockets");
}

void Power::FullPower::burst(uint8_t pin) {
    digitalWrite(pin, HIGH); delayMicroseconds(this->hi * this->sndlen);
    digitalWrite(pin, LOW);  delayMicroseconds(this->lo * this->sndlen);
}

void Power::send(const char* word) {
    unsigned long code = 0;
    uint8_t length = 0;
    for (const char* w = word; *w; w++) {
        code <<= 2l;
        switch(*w) {
            case '0': break;
            case 'F': code |= 1L; break;
            case '1': code |= 3L; break;
        }
        length += 2;
    }
    for (uint8_t _ = 0; _ < this->repeat; _++) {
        for (int16_t len = length-1; len >= 0; len--) {
            if (code & (1L << len)) {
                this->_one.burst(this->pin); this->txt.text("+", false);
            } else {
                this->_zer.burst(this->pin); this->txt.text("-", false);
            }
        }
        this->_syn.burst(this->pin); this->txt.text("", true);
    }
}

void Power::full(const char* address, bool power) {
    static char res[13];
    uint8_t pos = 0;
    for (uint8_t idx = 0; idx < 10; idx++) {
        res[pos++] = (address[idx] == '0') ? 'F' : '0';
    }
    res[pos++] = power ? '0' : 'F'; res[pos++] = power ? 'F' : '0';
    res[pos++] = _CHAR_IGNORE;
    this->txt.log("power", (power ? "full" : "null"));
    this->txt.llg("address", address);
    this->txt.llg("package", res);
    this->send(res);
}

String Power::key(uint8_t num) {
    if (num >= POWER_SWITCH) { return ""; }
    static char res[10]; sprintf(res, "power_%03d", num);
    return String(res);
}
String Power::address(uint8_t num, bool brief) {
    String pref = this->ini.get("power_family", POWER_FAMILY, true);
    String addr = this->ini.get(this->key(num), "11111", true);
    if (brief || !addr.length()) { return addr; }
    return this->txt.join(pref, addr);
}
String Power::name(uint8_t num) {
    return String(strtoul(this->address(num, true).c_str(), nullptr, 2), DEC);
}
String Power::look(String name) {
    if (!name.length() || name != String(name.toInt())) { return ""; }
    for (uint8_t idx = 0; idx < POWER_SWITCH; idx++) {
        if (this->name(idx) == name) { return this->address(idx); }
    }
    return "";
}
void Power::list(void) {
    this->txt.log("power", "socket list");
    for (uint8_t idx = 0; idx < POWER_SWITCH; idx++) {
        this->txt.llg(":", "#", String(idx), " ::");
        this->txt.llg("name", this->name(idx));
        this->txt.llg("short", this->address(idx, true));
        this->txt.llg("key", this->key(idx));
        this->txt.llg("address", this->address(idx, false));
    }
}

uint8_t Power::cmd_full(String text) {
    if (!text.length()) { return 1; } String addr = this->look(text);
    this->full((!addr.length() ? text : addr).c_str(), true); return 0;
}
uint8_t Power::cmd_null(String text) {
    if (!text.length()) { return 1; } String addr = this->look(text);
    this->full((!addr.length() ? text : addr).c_str(), false); return 0;
}
uint8_t Power::cmd_sock(String _) { this->list(); return 0; }
