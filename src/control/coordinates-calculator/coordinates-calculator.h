#pragma once
#include "core/sensors/BNO055/BNO055.h"
#include "control/camera-data/camera-data.h"

struct coordinates
{
    float x, y; // in cm
};

class CoordinatesCalculator
{
public:
    void calculate_coordinates(gyroData gyro_data, cameraData data);
    ::coordinates coordinates();

private:
    ::coordinates _coordinates;
};
