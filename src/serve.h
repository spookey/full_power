#ifndef __serve_h__
#define __serve_h__

#include <ESP8266WebServer.h>

#include "_init.h"
#include "cable.h"
#include "cover.h"
#include "light.h"
#include "power.h"

class Serve {
public:
    Serve(
        const Cable& msg, const Cover& net, const Light& led, const Power& pwr
    );

    void setup(void);
    void loop(void);

private:
    Cable msg;
    Cover net;
    Light led;
    Power pwr;
    ESP8266WebServer server;
    uint32_t requestCount = 0;

    void _action(void);
    String _load(String inner);
};

#endif
