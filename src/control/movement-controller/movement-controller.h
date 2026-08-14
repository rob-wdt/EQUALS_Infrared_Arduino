#pragma once

#include "core/motor/motor.h"
#include "core/sensors/BNO055/BNO055.h"
#include "utils/control/control.h"

struct motors
{
    Motor m1, m2, m3, m4;
};

struct motors_loc_angles
{
    int16_t m1_angle, m2_angle, m3_angle, m4_angle;
};

class MovementController
{
public:
    MovementController(motors motors, motors_loc_angles loc_angles, BNO055 gyro, Control *align_control);
    void init();
    void read_gyro();
    void set_velocity(int16_t lin_speed, int16_t move_angle, int16_t align_angle);
    void move();
    void stop();

private:
    motors _motors;
    motors_loc_angles _loc_angles;
    BNO055 _gyro;
    Control *_align_control;
};
