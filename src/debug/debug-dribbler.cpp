#include <Arduino.h>
#include "core/dribbler/dribbler.h"
#include "management/robot-type/robot-type.h"
#include "management/config/robot-config.h"

class DribblerTest
{
public:
    DribblerTest(Dribbler dribbler) : _dribbler(dribbler) {}
    void setup()
    {
        Serial.begin(115200);
        _dribbler.init();
    }
    void loop()
    {
        _dribbler.run();
        delay(2000);

        _dribbler.stop();
        delay(2000);
    }

private:
    Dribbler _dribbler;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

Dribbler dribbler(config.pins().dribbler, config.dribbler_config().min, config.dribbler_config().max);
DribblerTest test(dribbler);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
