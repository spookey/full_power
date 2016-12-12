#ifndef _serve_h_
#define _serve_h_

#include <ESP8266WebServer.h>

#include <_init.h>
#include <cable.h>
#include <light.h>
#include <power.h>

class Serve {
public:
    Serve(Cable& txt, Light& led, Power& ray);
    void setup(void);
    void loop(void);

protected:
    Cable& txt;
    Light& led;
    Power& ray;
    uint32_t requests = 0;

private:
    ESP8266WebServer srv = ESP8266WebServer(SERVE_HARBOR);

};

#endif
