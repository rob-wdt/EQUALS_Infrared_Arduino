#include "coordinates-calculator.h"
#include <Arduino.h>

void CoordinatesCalculator::calculate_coordinates(gyroData gyro_data, float rel_angle, float distance)
{
    // Calculate the new coordinates based on the gyro data, relative angle, and distance
    float angle_degrees = gyro_data.yaw + rel_angle;
    float angle_rad = radians(angle_degrees);

    _coordinates.x = distance * sin(angle_rad);
    _coordinates.y = distance * cos(angle_rad);
}

::coordinates CoordinatesCalculator::coordinates() { return _coordinates; }
