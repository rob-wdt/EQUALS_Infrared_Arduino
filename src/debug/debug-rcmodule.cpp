#include <Arduino.h>

#include "core/led/led.h"
#include "core/sensors/RoboCupModule/rcmodule.h"
#include "management/config/robot-config.h"

class RCModuleTest
{
public:
    RCModuleTest(RCModule module, Led led) : _module(module), _led(led) {}

    void setup()
    {
        _module.init();
        _led.init();
        Serial.begin(115200);

        Serial.println("RCModule test started");
        Serial.println("The LED follows the raw input: HIGH = on, LOW = off");
    }

    void loop()
    {
        _module.read();

        bool state = _module.start();

        if (state)
            _led.on();
        else
            _led.off();

        if (state == _last_state)
            return;

        _last_state = state;
        Serial.print("RCModule state:\t");
        Serial.println(state == HIGH ? "HIGH" : "LOW");
    }

private:
    RCModule _module;
    Led _led;
    bool _last_state = 0;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

RCModule module(config.pins().robocup_module);
Led led(config.pins().led1);
RCModuleTest test(module, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
