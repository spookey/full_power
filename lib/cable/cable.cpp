#include "cable.h"

Cable::Cable(unsigned long baud, SerialConfig conf)
: _baud(baud), _conf(conf), _prompt(*this), _commands(*this) {
}

void Cable::setup(void) {
    Serial.begin(this->_baud, this->_conf);
    do { delay(0.25); } while (!Serial); Serial.println();
}

void Cable::text(String text, bool nwl) {
    (nwl ? Serial.println(text) : Serial.print(text)); Serial.flush();
}
void Cable::raw(char data) { Serial.write(data); }
void Cable::raw(int data) { Serial.write(data); }

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
    const uint8_t len = text.length();
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

void Cable::sos(unsigned long wait) {
    String dit = this->join(this->fill(255), this->fill(255));
    String dah = this->join(dit, dit, dit, dit);
    for (uint8_t idx = 0; idx <= 2; idx++) {
        for (uint8_t _ = 0; _ <= 2; _++) {
            this->text((idx == 1) ? dah : dit, true); delay(512);
        }
        delay(512);
    }
    delay(wait);
}

bool Cable::Commands::append(Command& cmd) {
    for (uint8_t idx = 0; idx < CABLE_PROMPT_COUNT; idx++) {
        if (this->items[idx] == &cmd) { return true; }
    }
    if (this->index < CABLE_PROMPT_COUNT) {
        this->items[this->index] = &cmd;
        this->index++;
        return true;
    }
    return false;
}
bool Cable::append(String name, int8_t (*reply)(String)) {
    Command* cmd = (Command*) malloc(sizeof(Command));
    cmd->name = name; cmd->reply = reply;
    return this->_commands.append(*cmd);
}

int16_t Cable::Commands::launch(String line) {
    String progname, arguments;
    for (uint8_t idx = 0; idx < this->index; idx++) {
        progname = this->items[idx]->name;
        if (progname == line.substring(0, progname.length())) {
            arguments = line.substring(progname.length() + 1, line.length());
            arguments.trim();
            return (int16_t) this->items[idx]->reply(arguments);
        }
    }
    return -0x8000;
}
void Cable::Commands::help(void) {
    this->txt.log("help", "commands");
    for (uint8_t idx = 0; idx < this->index; idx++) {
        this->txt.llg(
            this->txt.join("#", String(idx)), this->items[idx]->name
        );
    }
    this->txt.llg(this->txt.join("#", String(this->index)), "help");
}

void Cable::Prompt::intro(void) {
    this->index = 0;
    this->txt.text(this->txt.pad("\nC:\\>", false, 6), false);
}
void Cable::Prompt::remove(uint8_t len) {
    do {
        if (this->index > 0) {
            this->txt.raw(CABLE_KEY_BACKSPACE);
            this->txt.raw(CABLE_KEY_SPACE);
            this->txt.raw(CABLE_KEY_BACKSPACE);
            this->index--;
        }
    } while(len--);
}
char Cable::Prompt::read(void) {
    char result = '\0';
    if (Serial.available()) { result = Serial.read(); }
    return result;
}
void Cable::Prompt::collect(char data) {
    if (data > 31 && data < 127) {
        if (this->index < CABLE_PROMPT_BUFFER) {
            this->buffer[this->index] = data;
            this->buffer[1 + this->index] = '\0';
            this->index++;
            this->txt.raw(data);
        }
    }
}
void Cable::Prompt::enter(void) {
    String line;
    if (this->index > 0) { line = this->buffer; line.trim(); }
    if (line.length() > 0) {
        int16_t code = this->txt._commands.launch(line);
        switch (code) {
            case 0: break;
            case -0x8000:
                this->txt._commands.help();
                break;
            default:
                this->txt.log("error", this->txt.join(
                    "\"", line, "\""
                ));
                this->txt.llg("code", this->txt.join(
                    String(code), " / ", String(code, HEX)
                ));
                break;
        }
    }
    this->intro();
}

void Cable::fetch(void) {
    char typed = this->_prompt.read();
    switch (typed) {
        case CABLE_KEY_ENTER:
            this->_prompt.enter();
            break;
        case CABLE_KEY_ESCAPE:
            this->_prompt.remove(this->_prompt.index);
            break;
        case CABLE_KEY_BACKSPACE:
            this->_prompt.remove();
            break;
        default:
            this->_prompt.collect(typed);
            break;
    }
}

void Cable::loop(void) {
    this->fetch();
}
