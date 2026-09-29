#include "role.h"
#include "config.h"

int Role::lin_speed() { return _lin_speed; }

int Role::move_angle() { return _move_angle; }

int Role::align_angle() { return _align_angle; }

void RoleAttacker::update(cameraData cam_data)
{
    _align_angle = 0;
    _lin_speed = SPEED;

    _move_angle = cam_data.ball_angle;
    if (cam_data.ball_angle > 0)
    {
        _move_angle += 90;
    }
    else
    {
        _move_angle -= 90;
    }
}

RoleDefender::RoleDefender(double move_coef) : _coef(_coef) {}

void RoleDefender::update(cameraData cam_data)
{
    _align_angle = 0;
    _lin_speed = _coef * cam_data.ball_angle;
    _move_angle = 90;
}
