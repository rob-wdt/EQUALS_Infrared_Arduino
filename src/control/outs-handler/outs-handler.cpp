#include "outs-handler.h"
#include <Arduino.h>

OutsHandler::OutsHandler(float hysteresis) : _hysteresis(hysteresis) {}

int8_t OutsHandler::_update_axis(int8_t state, float value, float high, float low)
{
    if (value >= high)
        return 1;
    if (value <= low)
        return -1;

    // Back inside: clear the out only after the hysteresis
    if (state == 1 && value < high - _hysteresis)
        return 0;
    if (state == -1 && value > low + _hysteresis)
        return 0;
    return state;
}

void OutsHandler::_update_zone(point coords, outsBounds outs)
{
    float side_depth = outs.goal_zone_half_width - fabs(coords.x); // how far inside the pocket from its side
    int8_t side = coords.x >= 0 ? 1 : -1;

    if (_zone == 0)
    {
        bool front_pocket = outs.front_corners > outs.front_goal && side_depth > 0 && coords.y >= outs.front_goal;
        bool rear_pocket = outs.rear_corners < outs.rear_goal && side_depth > 0 && coords.y <= outs.rear_goal;
        if (!front_pocket && !rear_pocket)
            return;

        _zone = front_pocket ? 1 : -1;
        float front_depth = front_pocket ? coords.y - outs.front_goal : outs.rear_goal - coords.y;
        // Leave the way the robot came in: came from the corner (the side is closer) -> sideways, otherwise back
        bool sideways = side_depth < front_depth;
        _zone_escape_x = sideways ? side : 0;
        _zone_escape_y = sideways ? 0 : -_zone;
        return;
    }

    // Back outside the pocket: clear it only after the hysteresis, like the other bounds
    float bound = _zone == 1 ? outs.front_goal : outs.rear_goal;
    bool out_front = _zone == 1 ? coords.y < bound - _hysteresis : coords.y > bound + _hysteresis;
    if (out_front || side_depth < -_hysteresis)
        _zone = 0;
}

void OutsHandler::handle(point coords, outsBounds outs)
{
    _out_x = _update_axis(_out_x, coords.x, outs.right, outs.left);
    _out_y = _update_axis(_out_y, coords.y, outs.front_corners, outs.rear_corners);
    _update_zone(coords, outs);

    // Opposite to the crossed bounds: RIGHT -> -90, LEFT -> 90, FRONT -> 180, REAR -> 0, RIGHT + FRONT -> -135
    float escape_x = -_out_x + (_zone ? _zone_escape_x : 0);
    float escape_y = -_out_y + (_zone ? _zone_escape_y : 0);
    if (escape_x != 0 || escape_y != 0)
        _escape_angle = round(degrees(atan2(escape_x, escape_y)));
}

bool OutsHandler::in_out() { return _out_x != 0 || _out_y != 0 || _zone != 0; }

int OutsHandler::escape_angle() { return _escape_angle; }

OutType OutsHandler::out_type()
{
    if (_out_x == 1)
        return OutType::RIGHT;
    if (_out_x == -1)
        return OutType::LEFT;
    if (_out_y == -1 || _zone == -1)
        return OutType::REAR_GOAL;
    if (_out_y == 1 || _zone == 1)
        return OutType::FRONT_GOAL;
    return OutType::NO;
}
