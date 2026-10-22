#include "coordinates-calculator.h"
#include <Arduino.h>

void CoordinatesCalculator::calculate_coordinates(gyroData gyro_data, cameraData data)
{
    float angle_rad_own = radians(gyro_data.yaw + data.own_goal_angle);
    float angle_rad_opp = radians(gyro_data.yaw + data.opp_goal_angle);

    float x_own = data.own_goal_distance * sin(angle_rad_own);
    float y_own = data.own_goal_distance * cos(angle_rad_own);

    float x_opp = data.opp_goal_distance * sin(angle_rad_opp);
    float y_opp = data.opp_goal_distance * cos(angle_rad_opp);

    // The camera angle is 0 in front of the robot, so each vector points from the robot to a goal: (goal - robot).
    // The goals are symmetric around the field center, so the average of the two vectors is minus the robot position
    _coordinates.x = -(x_own + x_opp) / 2;
    _coordinates.y = -(y_own + y_opp) / 2;
}

::coordinates CoordinatesCalculator::coordinates() { return _coordinates; }
