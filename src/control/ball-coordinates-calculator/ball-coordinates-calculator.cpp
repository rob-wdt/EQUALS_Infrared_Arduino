#include "ball-coordinates-calculator.h"

void BallCoordinatesCalculator::calculate(gyroData gyro_data, cameraData cam_data, ::coordinates robot_coords)
{
    float x_relative = cam_data.ball_distance * cos(radians(cam_data.ball_angle));
    float y_relative = cam_data.ball_distance * sin(radians(cam_data.ball_angle));

    float x_absolute = x_relative + robot_coords.x;
    float y_absolute = y_relative + robot_coords.y;

    _coords.x = x_absolute;
    _coords.y = y_absolute;
}

::coordinates BallCoordinatesCalculator::coordinates() { return _coords; }
