#pragma once

#include "core/motor/motor.h"
#include "core/sensors/BNO055/BNO055.h"
#include "utils/control/control.h"

enum MotorLocationAngle : int16_t
{
    ANGLE_45 = 45,
    ANGLE_135 = 135,
    ANGLE_225 = 225,
    ANGLE_315 = 315
};

struct motors
{
    Motor m1, m2, m3, m4;
};

struct motorsLocAngles
{
    MotorLocationAngle m1_angle, m2_angle, m3_angle, m4_angle;
};

struct motorsDirections
{
    MotorDirection m1_dir, m2_dir, m3_dir, m4_dir;
};

class MovementController
{
public:
    // deg of heading error from which the robot only turns, closer to align_angle it drives slower the further it is
    static const int TURN_FIRST_ANGLE = 60;

    MovementController(motors motors, motorsLocAngles loc_angles, Control *align_control);
    void init();
    void set_velocity(int16_t lin_speed, int16_t move_angle, int16_t align_angle, gyroData gyro_data);
    void move();
    void stop();

private:
    motors _motors;
    motorsLocAngles _loc_angles;
    Control *_align_control;
};
