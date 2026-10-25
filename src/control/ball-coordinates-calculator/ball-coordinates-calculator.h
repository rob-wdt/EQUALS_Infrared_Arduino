#pragma once

#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

class BallCoordinatesCalculator
{
public:
    void calculate(gyroData gyro_data, cameraData cam_data, ::coordinates robot_coords);
    ::coordinates coordinates();

private:
    ::coordinates _coords;
};
