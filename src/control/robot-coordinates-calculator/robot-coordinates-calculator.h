#pragma once
#include "core/sensors/BNO055/BNO055.h"
#include "control/camera-data/camera-data.h"
#include "utils/point/point.h"

class RobotCoordinatesCalculator
{
public:
    void calculate_coordinates(gyroData gyro_data, cameraData data);
    point coordinates();

private:
    point _coordinates;
};
