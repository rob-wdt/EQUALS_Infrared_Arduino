#include <Arduino.h>
#include "config.h"
#include "core/kicker/kicker.h"
#include "management/config/robot-config.h"

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

RobotViolet robot_type;
RobotConfig config(robot_type);

Kicker kicker(config.pins().solenoid, config.kicking_time_ms());
KickerTest test(kicker);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
