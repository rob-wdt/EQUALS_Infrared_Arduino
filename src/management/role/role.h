#pragma once

#include "control/camera-data-parser/camera-data-parser.h"

class Role
{
public:
    virtual void update(cameraData cam_data) = 0;
    int lin_speed();
    int move_angle();
    int align_angle();

protected:
    int _lin_speed;
    int _move_angle;
    int _align_angle;
};

class RoleAttacker : public Role
{
public:
    void update(cameraData cam_data) override;
};

class RoleDefender : public Role
{
public:
    RoleDefender(double move_koef);
    void update(cameraData cam_data) override;

private:
    double _koef;
};
