#include "shell.h"

Shell::Shell(Cable& txt)
: txt(txt), commands(txt), prompt(*this, txt) {}

Shell::Command::Command(String name, String help, int8_t (*reply)(String))
: name(name), help(help), reply(reply) {}

Shell::Commands::Commands(Cable& txt)
: txt(txt), index(0) {}

Shell::Prompt::Prompt(Shell& cmd, Cable& txt)
: cmd(cmd), txt(txt), index(0) {}

void Shell::setup(void) {}

void Shell::Prompt::intro(void) {
    this->index = 0;
    this->txt.text(this->txt.pad("\nC:\\>", false, 6), false);
}
void Shell::Prompt::remove(uint8_t len) {
    do {
        if (this->index > 0) {
            this->txt.raw(SHELL_KEY_BACKSPACE);
            this->txt.raw(SHELL_KEY_SPACE);
            this->txt.raw(SHELL_KEY_BACKSPACE);
        }
    } while(len--);
}
void Shell::Prompt::collect(char data) {
    if (data > 31 && data < 127) {
        if (this->index < SHELL_PROMPT_BUFFER) {
            this->buffer[this->index] = data;
            this->buffer[1 + this->index] = '\0';
            this->index++;
            this->txt.raw(data);
        }
    }
}
void Shell::Prompt::enter(void) {
    String line;
    if (this->index > 0) { line = this->buffer; line.trim(); }
    if (line.length() > 0) {
        int16_t code = this->cmd.commands.launch(line);
        switch (code) {
            case 0: break;
            case -0x8000: this->cmd.commands.help(); break;
            default:
                this->txt.log("error", this->txt.join("\"", line, "\""));
                this->txt.llg("code", this->txt.join(
                    String(code, DEC), " / ", String(code, HEX)
                ));
                break;
        }
    }
    this->intro();
}

int16_t Shell::Commands::launch(String line) {
    String progname, arguments;
    for (uint8_t idx = 0; idx < this->index; idx++) {
        progname = this->items[idx]->name;
        if (progname == line.substring(0, progname.length())) {
            arguments = line.substring(1 + progname.length(), line.length());
            arguments.trim();
            return (int16_t) this->items[idx]->reply(arguments);
        }
    }
    return -0x8000;
}
void Shell::Commands::help(void) {
    this->txt.log("avail", "commands");
    for (uint8_t idx = 0; idx < this->index; idx++) {
        this->txt.llg(this->items[idx]->name, this->items[idx]->help);
    }
    this->txt.llg("anything else", "this help");
}

bool Shell::Commands::append(Command& command) {
    for (uint8_t idx = 0; idx < SHELL_PROMPT_COUNT; idx++) {
        if (this->items[idx] == &command) { return true; }
    }
    if (this->index < SHELL_PROMPT_COUNT) {
        this->items[this->index] = &command;
        this->index++;
        return true;
    }
    return false;
}
bool Shell::append(String name, String help, int8_t (*reply)(String)) {
    Command* cmd = (Command*) malloc(sizeof(Command));
    cmd->name = name; cmd->help = help; cmd->reply = reply;
    return this->commands.append(*cmd);
}

void Shell::loop(void) {
    char typed = this->txt.collect();
    switch (typed) {
        case CABLE_KEY_ENTER:
            this->prompt.enter();
            break;
        case CABLE_KEY_ESCAPE:
            this->prompt.remove(this->prompt.index);
            break;
        case CABLE_KEY_BACKSPACE:
            this->prompt.remove();
            break;
        default:
            this->prompt.collect(typed);
            break;
    }
}
