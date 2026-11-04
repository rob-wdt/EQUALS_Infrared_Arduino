#include <Arduino.h>
#include "core/motor/motor.h"
#include "config.h"
#include "management/config/robot-config.h"

class MotorTest
{
public:
    MotorTest(Motor m1, Motor m2, Motor m3, Motor m4) : _m1(m1), _m2(m2), _m3(m3), _m4(m4) {}
    void setup()
    {
        _m1.init();
        _m2.init();
        _m3.init();
        _m4.init();
    }
    void loop(int speed)
    {
        _m1.set_speed(speed);
        _m2.set_speed(speed);
        _m3.set_speed(speed);
        _m4.set_speed(speed);

        _m1.run();
        _m2.run();
        _m3.run();
        _m4.run();
    }

private:
    Motor _m1;
    Motor _m2;
    Motor _m3;
    Motor _m4;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

Motor m1(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir);
Motor m2(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir);
Motor m3(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir);
Motor m4(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir);

MotorTest test(m1, m2, m3, m4);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.speed());
}
