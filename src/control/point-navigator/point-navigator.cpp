#include "point-navigator.h"

void PointNavigator::compute(coordinates robot_coords, coordinates target_coords)
{
    float delta_x = target_coords.x - robot_coords.x;
    float delta_y = target_coords.y - robot_coords.y;

    _data.move_angle = degrees(atan2(delta_y, delta_x));
}

navigationData PointNavigator::data() { return _data; }
