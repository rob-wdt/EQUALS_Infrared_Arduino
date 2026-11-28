#include "ball-motion-watcher.h"
#include <Arduino.h>

void BallMotionWatcher::update(bool ball_visible, point ball, float distance, int field_angle,
                               float position_tolerance, float distance_tolerance, int angle_tolerance)
{
    unsigned long now = millis();

    if (!ball_visible)
    {
        if (_visible)
            _lost_since = now;
        _visible = false;
        _has_ref = false;
        _still_ms = 0;
        _lost_ms = now - _lost_since;
        return;
    }

    _visible = true;
    _lost_ms = 0;

    float dx = ball.x - _ref.x;
    float dy = ball.y - _ref.y;
    // Compare bearings on a circle, including samples on opposite sides of 180 degrees.
    int angle_delta = abs((field_angle - _ref_angle) % 360);
    if (angle_delta > 180)
        angle_delta = 360 - angle_delta;

    if (!_has_ref || dx * dx + dy * dy > position_tolerance * position_tolerance ||
        fabs(distance - _ref_distance) > distance_tolerance || angle_delta > angle_tolerance)
    {
        _ref = ball;
        _ref_distance = distance;
        _ref_angle = field_angle;
        _has_ref = true;
        _still_since = now;
    }
    _still_ms = now - _still_since;
}

void BallMotionWatcher::reset()
{
    _has_ref = false;
    _visible = true; // so the "lost" counter restarts from now, not from boot
    _lost_since = millis();
    _still_ms = 0;
    _lost_ms = 0;
}

unsigned long BallMotionWatcher::still_ms() { return _still_ms; }

unsigned long BallMotionWatcher::lost_ms() { return _lost_ms; }
