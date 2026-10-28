#include "ball-coordinates-calculator.h"
#include "utils/math-vector/math-vector.h"

void BallCoordinatesCalculator::calculate(gyroData gyro_data, cameraData cam_data, point robot_coords)
{
    // The same axes as RobotCoordinatesCalculator: field heading = gyro yaw + camera angle
    _coords = MathVector::from_angle(robot_coords, gyro_data.yaw + cam_data.ball_angle, cam_data.ball_distance).end();
}

point BallCoordinatesCalculator::coordinates() { return _coords; }
