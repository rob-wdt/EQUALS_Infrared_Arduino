#pragma once
#include <stdint.h>
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

// cm, field coordinates. The allowed area is a rectangle with a pocket cut out in front of each goal:
//
//   front_corners -> +-------+         +-------+
//                    |       |  goal   |       |
//   front_goal ----> |       +---------+       |   <- the pocket is |x| < goal_zone_half_width
//                    |                         |
//                  left                      right
struct outsBounds
{
    float right;
    float left;
    float front_goal; // in front of the opponent goal
    float rear_goal;  // in front of the own goal
    float front_corners; // beside the opponent goal, = front_goal: no pocket
    float rear_corners;  // beside the own goal, = rear_goal: no pocket
    float goal_zone_half_width;
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
    void handle(point coords, outsBounds outs);
    bool in_out();
    // Move angle that leads back into the field, in a corner both bounds are handled at once (diagonally).
    // After the robot is back in the field it stays the same until the next out
    int escape_angle();
    // For printing, RIGHT/LEFT have priority over the goal sides (a pocket counts as its goal side)
    OutType out_type();

private:
    float _hysteresis;
    int8_t _out_x = 0; // 1 right, -1 left
    int8_t _out_y = 0; // 1 front corners, -1 rear corners
    int8_t _zone = 0;  // 1 in the pocket in front of the opponent goal, -1 the own goal
    // Way out of the pocket, chosen when the robot got in: back into the field or sideways into the corner
    int8_t _zone_escape_x = 0;
    int8_t _zone_escape_y = 0;
    int _escape_angle = 0;

    int8_t _update_axis(int8_t state, float value, float high, float low);
    void _update_zone(point coords, outsBounds outs);
};
