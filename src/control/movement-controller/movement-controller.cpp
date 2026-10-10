#include "movement-controller.h"
#include <Arduino.h>

MovementController::MovementController(motors motors, motorsLocAngles loc_angles, int min_pwm, Control *align_control)
    : _motors(motors), _loc_angles(loc_angles), _min_pwm(min_pwm), _align_control(align_control)
{
}

void MovementController::init()
{
    _motors.m1.init();
    _motors.m2.init();
    _motors.m3.init();
    _motors.m4.init();
}

void MovementController::set_velocity(int16_t lin_speed, int16_t move_angle, int16_t align_angle, gyroData gyro_data)
{
    int16_t error = align_angle - gyro_data.yaw;
    if (error > 180)
        error -= 360;
    else if (error < -180)
        error += 360;

    float angular_speed = _align_control->calculate(error);

    // Far from align_angle the robot turns first and drives after: otherwise on two of the wheels
    // driving and turning cancel each other out and the robot turns with the other two only
    float lin = lin_speed * constrain(1.0 - abs(error) / static_cast<float>(TURN_FIRST_ANGLE), 0.0, 1.0);

    Motor *m[4] = {&_motors.m1, &_motors.m2, &_motors.m3, &_motors.m4};
    int16_t loc[4] = {_loc_angles.m1_angle, _loc_angles.m2_angle, _loc_angles.m3_angle, _loc_angles.m4_angle};
    float speeds[4];
    float max_speed = 0;
    for (int i = 0; i < 4; i++)
    {
        speeds[i] = m[i]->direction() * (lin * cos(radians(move_angle + loc[i])) + angular_speed);
        max_speed = max(max_speed, fabs(speeds[i]));
    }

    // A wheel can't go faster than 255: slow all of them down equally so the direction stays the same
    float scale = max_speed > 255 ? 255 / max_speed : 1;
    for (int i = 0; i < 4; i++)
    {
        float speed = speeds[i] * scale;
        // Below _min_pwm a wheel doesn't turn and Motor::run() actually brakes it, the other wheels then
        // fight against it. Stretch every non-zero command to [_min_pwm, 255]
        if (fabs(speed) >= 3)
            speed = (speed > 0 ? 1 : -1) * (_min_pwm + fabs(speed) * (255 - _min_pwm) / 255.0);
        else
            speed = 0;
        m[i]->set_speed(speed);
    }
}

void MovementController::move()
{
    _motors.m1.run();
    _motors.m2.run();
    _motors.m3.run();
    _motors.m4.run();
}

void MovementController::stop()
{
    _motors.m1.stop();
    _motors.m2.stop();
    _motors.m3.stop();
    _motors.m4.stop();
}
