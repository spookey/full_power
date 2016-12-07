#ifndef _cable_h_
#define _cable_h_

#include <_init.h>

class Cable {
public:
    Cable(unsigned long baud, SerialConfig conf);
    void setup(void);
    void loop(void);

    void text(String text, bool nwl=false);
    void log(String name, String text);
    void llg(String name, String text);

    String join(String tx_a, String tx_b);

    void sos(unsigned long wait=2048);

    bool append(String name, int8_t (*reply)(String));

private:
    unsigned long _baud;
    SerialConfig _conf;

    String fill(uint8_t wdt=8, char chr=' ');
    String pad(
        String text, bool pre=true, uint8_t wdt=8, char fchr=' ', char schr='.'
    );

private:
    void raw(char data);
    void raw(int data);
    void fetch(void);

    struct Command {
        String name;
        int8_t (*reply)(String);
        Command(String name, int8_t (*reply)(String)): name(name), reply(reply) {};
    };
    struct Commands {
        Cable& txt;
        uint8_t index;
        Command* items[CABLE_PROMPT_COUNT];
        Commands(Cable &txt): txt(txt), index(0) {};
        bool append(Command& cmd);
        int16_t launch(String line);
        void help(void);
    };
    Commands _commands;

    struct Prompt {
        Cable& txt;
        uint8_t index;
        char buffer[1 + CABLE_PROMPT_BUFFER];
        Prompt(Cable &txt): txt(txt), index(0) {};
        void intro(void);
        void remove(uint8_t len=1);
        char read(void);
        void collect(char data);
        void enter(void);
    };
    Prompt _prompt;

public:
    String join(String tx_a, String tx_b, String tx_c) {
        return this->join(tx_a, this->join(tx_b, tx_c));
    }
    String join(String tx_a, String tx_b, String tx_c, String tx_d) {
        return this->join(tx_a, tx_b, this->join(tx_c, tx_d));
    }

    void log(String name, String tx_a, String tx_b) {
        this->log(name, this->join(tx_a, tx_b));
    }
    void log(String name, String tx_a, String tx_b, String tx_c) {
        this->log(name, this->join(tx_a, tx_b, tx_c));
    }
    void log(String name, String tx_a, String tx_b, String tx_c, String tx_d) {
        this->log(name, this->join(tx_a, tx_b, tx_c, tx_d));
    }

    void llg(String name, String tx_a, String tx_b) {
        this->llg(name, this->join(tx_a, tx_b));
    }
    void llg(String name, String tx_a, String tx_b, String tx_c) {
        this->llg(name, this->join(tx_a, tx_b, tx_c));
    }
    void llg(String name, String tx_a, String tx_b, String tx_c, String tx_d) {
        this->llg(name, this->join(tx_a, tx_b, tx_c, tx_d));
    }
};

#endif
