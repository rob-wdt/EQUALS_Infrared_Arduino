#pragma once
#include "control/coordinates-calculator/coordinates-calculator.h"

struct outsBounds
{
    float right;
    float left;
    float front_goal;
    float rear_goal;
};

enum OutType
{
    NO,
    RIGHT,
    LEFT,
    FRONT_GOAL,
    REAR_GOAL
};

class OutsHandler
{
public:
    void handle(coordinates coords, outsBounds outs);
    OutType out_type();

private:
    OutType _out_type = OutType::NO;
};
