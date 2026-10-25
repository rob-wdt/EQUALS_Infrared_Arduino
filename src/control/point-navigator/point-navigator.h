#pragma once

#include "control/movement-controller/movement-controller.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

// Recieves robot and target point coordinates and computes movement-necessary data
class PointNavigator
{
public:
    void compute(coordinates robot_coords, coordinates target_coords, movementData movement_data);
    movementData data();
private:
    movementData _data;
};
