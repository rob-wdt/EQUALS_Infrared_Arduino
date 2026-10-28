#pragma once

#include "control/movement-controller/movement-controller.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

struct navigationData
{
    float move_angle; // field heading to the target (0 = +y), subtract the gyro yaw to get it relative to the robot
    float distance;   // cm
};

// Recieves robot and target point coordinates and computes movement-necessary data
class PointNavigator
{
public:
    void compute(point robot_coords, point target_coords);
    navigationData data();

private:
    navigationData _data;
};
