#pragma once

#include "core/motor/motor.h"

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
    MovementController(motors motors, motorsLocAngles loc_angles);
    void init();
    // move_angle: relative to the robot front. angular_speed: PWM, > 0 makes the gyro yaw grow (HeadingController calculates it)
    void set_velocity(int lin_speed, int move_angle, float angular_speed);
    void move();
    void stop();

private:
    motors _motors;
    motorsLocAngles _loc_angles;
};
