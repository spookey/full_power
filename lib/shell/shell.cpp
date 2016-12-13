#include "shell.h"

Shell::Shell(Cable& txt)
: txt(txt) {}

bool Shell::add(Command& cmd) {
    for (uint8_t idx = 0; idx < SHELL_CMDLEN; idx++) {
        if (&this->items[idx] == &cmd) { return true; }
    }
    if (this->c_idx < SHELL_CMDLEN) {
        this->items[this->c_idx] = cmd;
        this->c_idx++;
        return true;
    }
}
void Shell::help(void) {
    this->txt.log("shell", "commands");
    for (uint8_t idx = 0; idx < this->c_idx; idx++) {
        this->txt.llg(this->items[idx].name, this->items[idx].help);
    }
    this->txt.llg("anything else", "this help");
    this->txt.llg(":", "#", String(this->launched), " ::");
    this->txt.llg("items", String(this->c_idx));
    this->txt.llg("free", String(SHELL_CMDLEN - this->c_idx));
}
void Shell::launch(String line) {
    String progname, arguments;
    this->launched++;
    uint8_t code = 1;
    for (uint8_t idx = 0; idx < this->c_idx; idx++) {
        progname = this->items[idx].name;
        if (progname == line.substring(0, progname.length())) {
            arguments = line.substring(1 + progname.length(), line.length());
            arguments.trim();
            code = this->items[idx].call(arguments);
            if (code == 0) { return; }
            this->txt.log("shell", this->txt.join("\'", line, "\'"));
            this->txt.llg("error", String(code, DEC));
            return;
        }
    }
    this->help();
}

void Shell::_intro(void) {
    this->p_idx = 0;
    this->txt.text(this->txt.pad(
        this->txt.join("\n", SHELL_PREFIX), false, SHELL_PRELEN, ' ', '%'
    ), false);
}
void Shell::_remove(uint8_t num) {
    for (; num; num--) {
        if (this->p_idx > 0) {
            this->p_idx--;
            this->txt.raw(_CODE_BACKSP);
            this->txt.raw(_CODE_FILLER);
            this->txt.raw(_CODE_BACKSP);
        }
    }
}
void Shell::_collect(char data) {
    if (data > 31 && data < 127) {
        if (this->p_idx < SHELL_PROMPT) {
            this->input[this->p_idx] = data;
            this->input[1 + this->p_idx] = _CHAR_IGNORE;
            this->p_idx++;
            this->txt.raw(data);
            this->txt.raw(_CODE_BACKSP);
            this->txt.raw(data);
        }
    }
}
void Shell::_enter(void) {
    String line;
    if (this->p_idx > 0) { line = this->input; line.trim(); }
    if (!!line.length()) { this->launch(line); }
    this->_intro();
}

void Shell::loop(void) {
    char typed = this->txt.collect();
    switch (typed) {
        case _CODE_RETURN: this->_enter();              break;
        case _CODE_ESCAPE: this->_remove(this->p_idx);  break;
        case _CODE_BACKSP: this->_remove();             break;
        default:           this->_collect(typed);       break;
    }
}
