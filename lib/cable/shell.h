#ifndef _shell_h_
#define _shell_h_

#include <_init.h>

class Shell {
public:
    const String name; const String help;

    template<typename T>
    Shell(T *base, uint8_t (T::*ident)(String), String name, String help="-")
    : name(name), help(help) {
        this->base = static_cast<void*>(base);
        memcpy(this->ident, (char*) &ident, sizeof(ident));
        this->caller = &Shell::invoke<T>;
    };
    uint8_t call(String param) {
        return this->caller(this->base, this->ident, param);
    }
private:
    void *base; char ident[0x21];
    uint8_t (*caller)(void*, char*, String);

    template<typename T>
    static uint8_t invoke(void* base, char* ident, String param) {
        T* obj = static_cast<T*>(base);
        uint8_t (T::*fnc)(String);
        memcpy((char*)&fnc, ident, sizeof(fnc));
        return (obj->*fnc)(param);
    };
};

#endif
