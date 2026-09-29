#include <Arduino.h>
#include "management/robot/robot.h"

class RobotTest
{
public:
    RobotTest(Robot robot) : _robot(robot) {}
    void setup()
    {
        _robot.init();
        _robot.calibrate_gyro();
    }
    void loop()
    {
        _robot.update();
    }

private:
    Robot _robot;
};

RobotViolet robot_type;
RobotConfig config(robot_type);
Robot robot(new RoleDefender(config.defender_move_coef()), robot_type);

RobotTest test(robot);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
