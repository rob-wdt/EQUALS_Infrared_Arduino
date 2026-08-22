#include "outs-handler.h"

void OutsHandler::handle(coordinates coords, outsBounds outs)
{
    if (coords.x >= outs.right)
    {
        _out_type = OutType::RIGHT;
        return;
    }
    else if (coords.x <= outs.left)
    {
        _out_type = OutType::LEFT;
        return;
    }
    else if (coords.y <= outs.rear_goal)
    {
        _out_type = OutType::REAR_GOAL;
        return;
    }
    else if (coords.y >= outs.front_goal)
    {
        _out_type = OutType::FRONT_GOAL;
        return;
    }
    else
    {
        _out_type = OutType::NO;
    }
}

OutType OutsHandler::out_type() { return _out_type; }
