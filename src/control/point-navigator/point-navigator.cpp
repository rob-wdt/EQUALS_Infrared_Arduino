#include "point-navigator.h"

void PointNavigator::compute(coordinates robot_coords, coordinates target_coords, movementData movement_data)
{
    float delta_x = target_coords.x - robot_coords.x;
    float delta_y = target_coords.y - robot_coords.y;

    _data.move_angle = degrees(atan2(delta_y, delta_x));
    _data.align_angle = movement_data.align_angle;
    _data.lin_speed = movement_data.lin_speed;
}

movementData PointNavigator::data() { return _data; }
