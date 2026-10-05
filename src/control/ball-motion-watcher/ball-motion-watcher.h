#pragma once

#include "control/coordinates-calculator/coordinates-calculator.h"

// Tracks how long the ball has been standing still (in field coordinates) and how long it has been out of sight
class BallMotionWatcher
{
public:
    void update(bool ball_visible, coordinates ball, float epsilon);
    void reset();
    unsigned long still_ms();
    unsigned long lost_ms();

private:
    coordinates _ref = {0, 0};
    bool _visible = false;
    bool _has_ref = false;
    unsigned long _still_since = 0;
    unsigned long _lost_since = 0;
    unsigned long _still_ms = 0;
    unsigned long _lost_ms = 0;
};
