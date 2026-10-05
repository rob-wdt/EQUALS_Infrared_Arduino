#include <Arduino.h>
#include "core/motor/motor.h"
#include "config.h"
#include "management/config/robot-config.h"

class MotorTest
{
public:
    MotorTest(int pin1, int pin2, MotorDirection direction) : _motor(pin1, pin2, direction) {}
    void setup()
    {
        _motor.init();
    }
    void loop(int speed)
    {
        _motor.set_speed(speed);
        _motor.run();
    }

private:
    Motor _motor;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

MotorTest test(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.speed());
}
