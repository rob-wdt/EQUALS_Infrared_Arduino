#include "point-navigator.h"
#include "utils/math-vector/math-vector.h"

void PointNavigator::compute(point robot_coords, point target_coords)
{
    MathVector to_target(robot_coords, target_coords);

    _data.move_angle = to_target.angle();
    _data.distance = to_target.length();
}

navigationData PointNavigator::data() { return _data; }
