#ifndef _shell_h_
#define _shell_h_

#include <_init.h>
#include <cable.h>

class Shell {
public:
    Shell(Cable& txt);
    void loop(void);

protected:
    Cable& txt;
    uint8_t c_idx = 0; /* registered command counter */
    uint8_t p_idx = 0; /* prompt input counter */
    uint16_t launched = 0;

private:
    struct Command {
        void *base; char ident[0x21];
        uint8_t (*caller)(void*, char*, String);
        String name; String help;

        uint8_t call(String text) {
            return this->caller(this->base, this->ident, text);
        }
        template<typename T>
        static uint8_t invoke(void* base, char* ident, String text) {
            T* obj = static_cast<T*>(base);
            uint8_t (T::*fnc)(String);
            memcpy((char*) &fnc, ident, sizeof(fnc));
            return (obj->*fnc)(text);
        }
    };

    bool add(Command& cmd);

public:
    template<typename T>
    bool add(T *base, uint8_t (T::*ident)(String),
            String name, String help="-") {
        Command* cmd = new Command;
        cmd->base = static_cast<void*>(base);
        memcpy(cmd->ident, (char*) &ident, sizeof(ident));
        cmd->caller = &Command::invoke<T>;
        cmd->name = name;
        cmd->help = help;
        return this->add(*cmd);
    }

private:
    void help(void);
    void launch(String line);
    Command* items = new Command[SHELL_CMDLEN];

    void _intro(void);
    void _remove(uint8_t num=1);
    void _collect(char data);
    void _enter(void);
    char input[1 + SHELL_PROMPT];

public:
    uint16_t get_launched(void) { return this->launched; }
};

#endif
