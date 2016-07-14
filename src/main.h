#ifndef __main_h__
#define __main_h__

#include <ESP8266WebServer.h>

#include "_init.h"
#include "cable.h"
#include "cover.h"
#include "light.h"
// #include "power.h"

void setup(void);
void loop(void);

void _index(void);
void _notFound(void);

#endif
