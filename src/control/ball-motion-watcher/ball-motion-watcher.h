#pragma once

#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"

// Tracks how long the ball measurements stay within tolerances and how long the ball is out of sight
class BallMotionWatcher
{
public:
    void update(bool ball_visible, point ball, float distance, int field_angle,
                float position_tolerance, float distance_tolerance, int angle_tolerance);
    void reset();
    unsigned long still_ms();
    unsigned long lost_ms();

private:
    point _ref = {0, 0};
    float _ref_distance = 0;
    int _ref_angle = 0;
    bool _visible = false;
    bool _has_ref = false;
    unsigned long _still_since = 0;
    unsigned long _lost_since = 0;
    unsigned long _still_ms = 0;
    unsigned long _lost_ms = 0;
};
