#ifndef __init_h_
#define __init_h_

#include <Arduino.h>

#define CABLE_BAUDRT        115200
#define CABLE_CONFIG        SERIAL_8N1

#define LIGHT_LED_BB        D1
#define LIGHT_LED_GG        D2
#define LIGHT_LED_RR        D3

#define SHELL_CMDLEN        16
#define SHELL_PREFIX        "C:\\>"
#define SHELL_PRELEN        6
#define SHELL_PROMPT        64

#define STORE_CONFIG        "/config_file"
#define STORE_INILEN        8
#define STORE_RASTER        16

#endif
