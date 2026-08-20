#include "coordinates-calculator.h"
#include <Arduino.h>

void CoordinatesCalculator::calculate_coordinates(gyroData gyro_data, float rel_angle, float distance)
{
    // Calculate the new coordinates based on the gyro data, relative angle, and distance
    float angle_rad = radians(gyro_data.yaw + rel_angle);
    _coordinates.x = distance * cos(angle_rad);
    _coordinates.y = distance * sin(angle_rad);
}

::coordinates CoordinatesCalculator::coordinates() { return _coordinates; }
