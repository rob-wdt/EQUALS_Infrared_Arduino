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
        Serial.print("Current state:\t");
        Serial.println(_robot.current_state()->name());
    }

private:
    Robot _robot;
};

RobotViolet robot_type;
GoalColor own_goal = GoalColor::BLUE;
Robot robot(RoleKind::ATTACKER, robot_type, own_goal, false);

RobotTest test(robot);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
