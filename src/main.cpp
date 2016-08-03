#include "main.h"

Cable msg = Cable(115200, SERIAL_8N1);
Cover net = Cover(msg, COVER_NET_SSID, COVER_NET_SECRET, COVER_NET_HOSTNAME);
Light led = Light(msg, LIGHT_LED_PINS);
Power pwr = Power(msg, POWER_SEND_PIN);
Serve srv = Serve(msg, net, led, pwr);

void setup(void) {
    randomSeed(analogRead(A0));
    srv.setup();
}

void loop(void) {
    srv.loop();
    delay(250);
}
