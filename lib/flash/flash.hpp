#ifndef _flash_hpp_
#define _flash_hpp_

#include <FS.h>

#include <_init.hpp>
#include <cable.hpp>
#include <shell.hpp>

class Flash {
public:
    Flash(Cable& txt, Shell& exe);
    void setup(void);

protected:
    Cable& txt;
    Shell& exe;
    const String filename = FLASH_CONFIG;

private:
    struct Blob { String key; String val; };
    uint8_t index = 0;
    Blob* items = new Blob[FLASH_INILEN];

    bool dump(bool action=true);
    String pickle(Blob data);
    String pickle(void);
    bool load(bool action=true);
    bool unpickle(String line);

    bool add(String key, String value);
    bool set(String key, String value);

    uint8_t cmd_confdrop(String _);
    uint8_t cmd_confdump(String _);
    uint8_t cmd_confload(String _);
    uint8_t cmd_confshow(String _);
    uint8_t cmd_conf(String text);

public:
    String get(String key, String fallback="", bool create=false);
};

#endif
