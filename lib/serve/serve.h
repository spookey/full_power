#ifndef _serve_h_
#define _serve_h_

#include <ESP8266WebServer.h>

#include <_init.h>
#include <cable.h>
#include <cover.h>
#include <light.h>
#include <power.h>
#include <shell.h>

class Serve {
public:
    Serve(Cable& txt, Shell& exe, Cover& net, Light& led, Power& ray);
    void setup(void);
    void loop(void);

protected:
    Cable& txt;
    Shell& exe;
    Cover& net;
    Light& led;
    Power& ray;
    uint32_t requests = 0;

private:
    ESP8266WebServer srv = ESP8266WebServer(SERVE_HARBOR);

    struct Json {
        Cable& txt;
        bool verbose = false;
        String store = "";

        Json(Cable& txt);
        void add(String key, String val, bool raw=false);
        void add(String key, int val) { this->add(key, String(val), true); }
        String show(void);

    };

    String index(void);
    String light(String text, bool fade);
    String power(String text, bool full);
    String stats(void);

    class Index : public RequestHandler {
    public:
        Index(Serve& web);
    protected:
        Serve& web;
        String _index = "/";
        String _light = "/light/";
        String _power = "/power/";
        String _stats = "/stats";
    public:
        bool canHandle(HTTPMethod meth, String req);
        bool handle(ESP8266WebServer& srv, HTTPMethod meth, String req);
        bool canUpload(String _) { return false; }
    };

    uint8_t cmd_stats(String text);

};

#endif
