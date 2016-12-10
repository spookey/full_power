#ifndef __init_h_
#define __init_h_

#include <Arduino.h>

#define CABLE_BAUDRT        115200
#define CABLE_CONFIG        SERIAL_8N1

#define COVER_H_NAME        "device"        // host name if no config file
#define COVER_REPEAT        4               // connection attempts
#define COVER_REPLAY        240000          // delay in ms before next attempt
#define COVER_W_PASS        "secret"        // wifi ssid if no config file
#define COVER_W_SSID        "aether"        // wifi pass if no config file

#define LIGHT_LED_BB        D1
#define LIGHT_LED_GG        D2
#define LIGHT_LED_RR        D3

#define SERVE_HARBOR        80              // the port :)

#define SHELL_CMDLEN        16              // how many commands may be added
#define SHELL_PREFIX        "C:\\>"
#define SHELL_PRELEN        6               // max prompt width
#define SHELL_PROMPT        64              // characters to capture

#define STORE_CONFIG        "/config_file"  // config filename
#define STORE_INILEN        8               // how many values may be stored
#define STORE_RASTER        16              // width of key & value in text

#define _CHAR_IGNORE        '\0'
#define _CODE_BACKSP        8
#define _CODE_ESCAPE        27
#define _CODE_FILLER        32
#define _CODE_RETURN        13

#endif
