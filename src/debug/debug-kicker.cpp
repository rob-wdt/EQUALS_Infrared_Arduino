#include <Arduino.h>
#include "config.h"
#include "core/kicker/kicker.h"
#include "management/config/robot-config.h"
#include "core/sensors/button/button.h"

class KickerTest
{
public:
    KickerTest(Kicker kicker, Button btn) : _kicker(kicker), _btn(btn) {}
    void setup()
    {
        Serial.begin(115200);
        _kicker.init();
        _btn.init();
        _btn.reset();
        _kicking_timer.set_time_ms(1000);
    }
    void loop()
    {
        _btn.wait_click();
        _kicker.kick();
        _kicking_timer.reset();
        delay(200);
        _kicker.unkick();
    }

private:
    Kicker _kicker;
    Timer _kicking_timer;
    Button _btn;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

Kicker kicker(config.pins().solenoid, config.kicking_time_ms());
Button btn(config.pins().btn1);
KickerTest test(kicker, btn);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
