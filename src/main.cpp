#include <Arduino.h>
#include "config.h"
#include "management/robot/robot.h"

RoleKind start_role = RoleKind::ATTACKER;
RobotViolet robot_type;

Robot robot(start_role, robot_type);

void setup()
{
    robot.init();
    robot.calibrate_gyro();
}

void loop()
{
    robot.update();
}
