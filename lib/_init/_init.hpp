#ifndef __init_hpp_
#define __init_hpp_

#include <Arduino.h>

#define CABLE_BAUDRT        115200          // serial speed
#define CABLE_CONFIG        SERIAL_8N1      // serial mode

#define COVER_H_NAME        "device"        // host name if no config file
#define COVER_REPEAT        4               // connection attempts
#define COVER_REPLAY        240000          // delay in ms before next attempt
#define COVER_W_PASS        "secret"        // wifi ssid if no config file
#define COVER_W_SSID        "aether"        // wifi pass if no config file

#define FLASH_CONFIG        "/config_file"  // config filename
#define FLASH_INILEN        16              // how many values may be stored
#define FLASH_RASTER        16              // width of key & value in text

#define LIGHT_LED_BB        D1
#define LIGHT_LED_GG        D2
#define LIGHT_LED_RR        D3

#define POWER_FAMILY        "01101"         // prefix for socket addresses
#define POWER_REPEAT        10              // how often a command is repeated
#define POWER_SWITCH        12              // how many switches to reserve
#define POWER_TX_PIN        D5

#define SERVE_HARBOR        80              // the port :)
#define SERVE_UPDATE        120000          // background refresh each ms

#define SHELL_CMDLEN        16              // how many commands may be added
#define SHELL_PREFIX        "C:\\>"         // commandline prefix
#define SHELL_PROMPT        64              // characters to capture

#define _CHAR_IGNORE        '\0'            // character repr nullbyte
#define _CODE_BACKSP        8               // character code backspace
#define _CODE_ESCAPE        27              // character code escape key
#define _CODE_FILLER        32              // character code space
#define _CODE_RETURN        13              // character code return key

#endif
