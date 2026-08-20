#pragma once
#include "core/sensors/BNO055/BNO055.h"

struct coordinates
{
    float x, y; // in cm
};

class CoordinatesCalculator
{
public:
    void calculate_coordinates(gyroData gyro_data, float rel_angle, float distance);
    ::coordinates coordinates();

private:
    ::coordinates _coordinates;
};
