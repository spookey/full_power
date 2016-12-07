#ifndef __init_h_
#define __init_h_

#include <Arduino.h>

#define CABLE_BAUD          115200
#define CABLE_CONF          SERIAL_8N1

#define LIGHT_LED_BB        D1
#define LIGHT_LED_GG        D2
#define LIGHT_LED_RR        D3

#define SHELL_KEY_BACKSPACE 8
#define SHELL_KEY_ENTER     13
#define SHELL_KEY_ESCAPE    27
#define SHELL_KEY_SPACE     32
#define SHELL_PROMPT_BUFFER 64
#define SHELL_PROMPT_COUNT  16

#endif
