#include "main.hpp"

Cable txt = Cable();
Shell exe = Shell(txt);
Flash ini = Flash(txt, exe);
Cover net = Cover(txt, exe, ini);
Light led = Light(txt, exe);
Power ray = Power(txt, exe, ini);
Serve web = Serve(txt, exe, net, led, ray);

void setup(void) {
    txt.setup();
    ini.setup();
    net.setup();
    led.setup();
    ray.setup();
    web.setup();
}
void loop(void) {
    exe.loop();
    net.loop();
    web.loop();
}
