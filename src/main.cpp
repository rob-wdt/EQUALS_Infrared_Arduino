#include <Arduino.h>
#include "management/robot/robot.h"

RoleKind start_role = RoleKind::ATTACKER_SIMPLE;
GoalColor own_goal = GoalColor::YELLOW;
RobotViolet robot_type;

Robot robot(start_role, robot_type, own_goal, false);

void setup()
{
    robot.init();
    robot.calibrate_gyro();
}

void loop()
{
    robot.update();
}
