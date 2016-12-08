#ifndef _shell_h_
#define _shell_h_

#include <_init.h>
#include <cable.h>

class Shell {
public:
    Shell(Cable& txt);

    void setup(void);
    void loop(void);

    bool append(String name, String help, int8_t (*reply)(String));
    bool append(String name, int8_t (*reply)(String)) {
        return this->append(name, "-", reply);
    }


private:
    Cable& txt;

    struct Command {
        String name;
        String help;
        int8_t (*reply)(String);

        Command(String name, String help, int8_t (*reply)(String));
    };
    struct Commands {
        Cable& txt;
        uint8_t index;
        Command* items[SHELL_PROMPT_COUNT];

        Commands(Cable& txt);
        bool append(Command& command);
        int16_t launch(String line);
        void help(void);
    };
    struct Prompt {
        Shell& cmd;
        Cable& txt;
        uint8_t index;
        char buffer[1 + SHELL_PROMPT_BUFFER];

        Prompt(Shell& cmd, Cable& txt);
        void intro(void);
        void remove(uint8_t num=1);
        void collect(char data);
        void enter(void);
    };

    Commands commands;
    Prompt prompt;
};

#endif
