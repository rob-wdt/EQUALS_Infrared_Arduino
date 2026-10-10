#pragma once
#include <stdint.h>
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
    // hysteresis: cm the robot has to come back inside a bound before that out is cleared,
    // otherwise coordinate noise near the bound switches the out on and off every frame
    OutsHandler(float hysteresis);
    void handle(coordinates coords, outsBounds outs);
    bool in_out();
    // Move angle that leads back into the field, in a corner both bounds are handled at once (diagonally).
    // After the robot is back in the field it stays the same until the next out
    int escape_angle();
    // For printing, RIGHT/LEFT have priority over the goal sides
    OutType out_type();

private:
    float _hysteresis;
    int8_t _out_x = 0; // 1 right, -1 left
    int8_t _out_y = 0; // 1 front goal, -1 rear goal
    int _escape_angle = 0;

    int8_t _update_axis(int8_t state, float value, float high, float low);
};
