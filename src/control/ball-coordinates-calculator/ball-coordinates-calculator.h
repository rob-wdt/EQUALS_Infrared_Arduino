#pragma once

#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

class BallCoordinatesCalculator
{
public:
    void calculate(gyroData gyro_data, cameraData cam_data, point robot_coords);
    point coordinates();

private:
    point _coords;
};
