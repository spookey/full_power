#include "flash.hpp"

Flash::Flash(Cable& txt, Shell& exe)
: txt(txt), exe(exe) {}

void Flash::setup(void) {
    if (!SPIFFS.begin()) { this->txt.sos("flash error", true); }
    this->load();
    this->exe.add(this, &Flash::cmd_confdrop, "cdrop", "drop config file");
    this->exe.add(this, &Flash::cmd_confdump, "cdump", "config -> file");
    this->exe.add(this, &Flash::cmd_confload, "cload", "file -> config");
    this->exe.add(this, &Flash::cmd_confshow, "cshow", "show config file");
    this->exe.add(this, &Flash::cmd_conf, "conf", "config utility");
}

bool Flash::dump(bool action) {
    uint32_t size; String conf;
    this->txt.log("flash", "dump");
    if (action) {
        conf = this->pickle();
        size = conf.length();
        if (!size) { this->txt.llg("no input", "abort"); return false; }
        this->txt.llg("input size", this->txt.join(String(size), " bytes"));
    }
    this->txt.llg("filename", this->filename);
    this->txt.llg("exists", (SPIFFS.exists(this->filename) ? "yes" : "no"));
    File file = SPIFFS.open(this->filename, "w+");
    if (!file) { this->txt.llg("open", "failed"); return false; }
    if (!file.seek(0, SeekSet)) {
        this->txt.llg("rewind", "failed"); return false;
    }
    this->txt.llg("ready", "writing ->");
    this->txt.text(conf, true);
    size = file.println(conf);
    file.close();
    this->txt.llg("written", this->txt.join(String(size), " bytes"));
    return true;
}

String Flash::pickle(Blob data) {
    return this->txt.join(
        this->txt.pad(data.key, false, FLASH_RASTER, ' ', '%'),
        this->txt.pad(data.val, true, FLASH_RASTER, ' ', '%')
    );
}
String Flash::pickle() {
    String conf = this->txt.join("# created - ", String(millis()));
    for (uint8_t idx = 0; idx < this->index; idx++) {
        conf = this->txt.join(conf, "\n", this->pickle(this->items[idx]));
    }
    conf = this->txt.join(conf, "\n");
    return conf;
}

bool Flash::load(bool action) {
    String line;
    this->txt.log("flash", "load");
    this->txt.llg("filename", this->filename);
    if (!SPIFFS.exists(this->filename)) {
        this->txt.llg("does not exist", "abort"); return false;
    }
    File file = SPIFFS.open(this->filename, "r");
    if (!file) { this->txt.llg("open", "failed"); return false; }
    if (!file.seek(0, SeekSet)) {
        this->txt.llg("rewind", "failed"); return false;
    }
    this->txt.llg("ready", "reading ->");
    while (file.available()) {
        line = file.readStringUntil('\n');
        if (action) {
            this->txt.llg((this->unpickle(line) ? "ok" : "ignored"), line);
        } else { this->txt.text(line, true); }
    }
    file.close();
    return true;
}
bool Flash::unpickle(String line) {
    String key, val; line.trim();
    const uint32_t len = line.length();
    if(!len) { this->txt.text("[empty]", true); return false; }
    if (line.startsWith("#")) {
        this->txt.text("[comment]", true); return false;
    }
    if(len < (2 * FLASH_RASTER)) {
        this->txt.text("[too short]", true); return false;
    }
    key = line.substring(0, FLASH_RASTER);
    val = line.substring(1 + FLASH_RASTER, line.length());
    key.trim(); val.trim();
    if(!key.length() || !val.length()) {
        this->txt.text("[no pair]", true); return false;
    }
    this->txt.text("[\\o/]", true);
    return this->add(key, val);
}

bool Flash::add(String key, String val) {
    key.trim();
    if (!key.length()) { return false; }
    if (this->set(key, val)) { return true; }
    if (this->index < FLASH_INILEN) {
        this->items[this->index] = Blob({key, val});
        this->index++;
        return true;
    }
    return false;
}
bool Flash::set(String key, String val) {
    for (uint8_t idx = 0; idx < this->index; idx++) {
        if (this->items[idx].key == key) {
            this->items[idx].val = val;
            return true;
        }
    }
    return false;
}
String Flash::get(String key, String fallback, bool create) {
    for (uint8_t idx = 0; idx < this->index; idx++) {
        if (this->items[idx].key == key) {
            return this->items[idx].val;
        }
    }
    if (create) {
        if (!this->add(key, fallback)) { return ""; }
    }
    return fallback;
}

uint8_t Flash::cmd_confdump(String _) {
    this->txt.log("flash", "conf dump");
    return (this->dump() ? 0 : 1);
}
uint8_t Flash::cmd_confdrop(String _) {
    this->txt.log("flash", "conf drop");
    return (this->dump(false) ? 0 : 1);
}
uint8_t Flash::cmd_confload(String _) {
    this->txt.log("flash", "conf load");
    return (this->load() ? 0 : 1);
}
uint8_t Flash::cmd_confshow(String _) {
    this->txt.log("flash", "conf show");
    return (this->load(false) ? 0 : 1);
}
uint8_t Flash::cmd_conf(String text) {
    uint8_t len = text.length();
    String key, val;

    for (uint8_t sdx = 0; sdx < len; sdx++) {
        if (sdx != len && text.charAt(sdx) == ' ') {
            key = text.substring(0, sdx);
            val = text.substring(1 + sdx, len);
            break;
        }
    }

    if (val.length() && key.length()) {
        this->txt.log("flash", "conf set");
        this->txt.llg(key, val);
        if (!this->set(key, val)) { return 1; }
        this->txt.llg("state", "ok");
    } else if (len) {
        this->txt.log("flash", "conf get");
        this->txt.llg(text, this->get(text, "-"));
    } else {
        this->txt.log("flash", "conf all");
        for (uint8_t idx = 0; idx < this->index; idx++) {
            this->txt.llg(this->items[idx].key, this->items[idx].val);
        }
        this->txt.llg("items", String(this->index));
        this->txt.llg("free", String(FLASH_INILEN - this->index));
    }
    return 0;
}
