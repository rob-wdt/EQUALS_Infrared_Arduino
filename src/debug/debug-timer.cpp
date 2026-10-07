#include <Arduino.h>
#include "config.h"
#include "control/timer/timer.h"

class TimerTest
{
public:
    TimerTest(Timer timer) : _timer(timer) {}
    void setup()
    {
        Serial.begin(115200);
        _timer.set_time_ms(5000);
    }
    void loop()
    {
        if (_timer.time_is_up())
        {
            Serial.println("Time is up");
            _timer.reset();
        }
    }

private:
    Timer _timer;
};

Timer timer;
TimerTest test(timer);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
