#include <Arduino.h>
#include "config.h"
#include "core/kicker/kicker.h"

class KickerTest
{
public:
    KickerTest(Kicker kicker) : _kicker(kicker) {}
    void setup()
    {
        Serial.begin(115200);
        _kicker.init();
        _kicking_timer.set_time_ms(3000);
    }
    void loop()
    {
        if (_kicking_timer.time_is_up())
        {
            _kicker.kick();
            _kicking_timer.reset();
        }
        _kicker.unkick();
    }

private:
    Kicker _kicker;
    Timer _kicking_timer;
};

Kicker kicker(Pin::PIN_SOLENOID, KICKING_TIME_MS);
KickerTest test(kicker);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
