#include "main.h"

Cable msg(115200, SERIAL_8N1);
Cover net(msg, COVER_NET_SSID, COVER_NET_SECRET, COVER_NET_HOSTNAME);
Light led(msg, LIGHT_LED_PINS);

ESP8266WebServer server;

uint32_t requestCount = 0;
uint32_t ticker = 0;

void setup(void) {
    randomSeed(analogRead(A0));
    net.dialup();
    server.on("/", _index);
    server.onNotFound(_notFound);
    server.begin();
    led.initTune();
    led.fade(led.someColor());
}

void loop(void) {
    ticker++;
    server.handleClient();
    if (ticker == 0) { net.info(); }
}

void _index(void) {
    requestCount++;
    led.flashTwice();
    server.send(200, "text/plain", "Color");
}

void _notFound(void) {
    requestCount++;
    String response("Not found!\n\n");
    switch (server.method()) {
        case HTTP_GET:
            response += "GET"; break;
        case HTTP_POST:
            response += "POST"; break;
        case HTTP_PUT:
            response += "PUT"; break;
        default: break;
    }
    response += String("\t'" + server.uri() + "'\n");
    for(uint8_t a = 0; a < server.args(); a++) {
        response += String(server.argName(a) + "\t'" + server.arg(a) + "'\n");
    }

    response += String("\nRequests\t" + String(requestCount) + "\n");
    response += String("Reconnects\t" + String(net.reconnects()) + "\n");
    server.send(404, "text/plain", response);
}
