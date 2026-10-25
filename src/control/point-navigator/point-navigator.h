#pragma once

#include "control/movement-controller/movement-controller.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

struct navigationData
{
    float move_angle;
};

// Recieves robot and target point coordinates and computes movement-necessary data
class PointNavigator
{
public:
    void compute(coordinates robot_coords, coordinates target_coords);
    navigationData data();
private:
    navigationData _data;
};
