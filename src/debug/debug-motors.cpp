#include <Arduino.h>
#include "core/motor/motor.h"
#include "config.h"
#include "management/config/robot-config.h"

class MotorTest
{
public:
    MotorTest(Motor motor) : _motor(motor) {}
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

Motor m1(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir);
Motor m2(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir);
Motor m3(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir);
Motor m4(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir);

MotorTest test(m1);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.speed());
}
