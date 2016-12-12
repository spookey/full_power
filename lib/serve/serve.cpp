#include "serve.h"

const String SERVE_TEST = String(
#include "test.html"
);

Serve::Serve(Cable& txt, Light& led, Power& ray)
: txt(txt), led(led), ray(ray) {}

void Serve::setup(void) {
    this->srv.on("/", [this](){
        this->requests++;
        String result = SERVE_TEST;
        result.replace("__req__", String(this->requests));
        this->srv.send(200, "text/html", result);
    });
    this->srv.begin();
}
void Serve::loop(void) { this->srv.handleClient(); }

