#include "ball-motion-watcher.h"
#include <Arduino.h>

void BallMotionWatcher::update(bool ball_visible, coordinates ball, float epsilon)
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
    if (!_has_ref || dx * dx + dy * dy > epsilon * epsilon)
    {
        _ref = ball;
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
