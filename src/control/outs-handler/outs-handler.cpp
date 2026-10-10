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

void OutsHandler::handle(coordinates coords, outsBounds outs)
{
    _out_x = _update_axis(_out_x, coords.x, outs.right, outs.left);
    _out_y = _update_axis(_out_y, coords.y, outs.front_goal, outs.rear_goal);

    // Opposite to the crossed bounds: RIGHT -> -90, LEFT -> 90, FRONT -> 180, REAR -> 0, RIGHT + FRONT -> -135
    if (in_out())
        _escape_angle = round(degrees(atan2(static_cast<float>(-_out_x), static_cast<float>(-_out_y))));
}

bool OutsHandler::in_out() { return _out_x != 0 || _out_y != 0; }

int OutsHandler::escape_angle() { return _escape_angle; }

OutType OutsHandler::out_type()
{
    if (_out_x == 1)
        return OutType::RIGHT;
    if (_out_x == -1)
        return OutType::LEFT;
    if (_out_y == -1)
        return OutType::REAR_GOAL;
    if (_out_y == 1)
        return OutType::FRONT_GOAL;
    return OutType::NO;
}
