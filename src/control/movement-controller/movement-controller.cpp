#include "movement-controller.h"
#include <Arduino.h>

MovementController::MovementController(motors motors, motors_loc_angles loc_angles, Control *align_control)
    : _motors(motors), _loc_angles(loc_angles), _align_control(align_control)
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
    int16_t angular_speed = _align_control->calculate(align_angle - gyro_data.yaw);

    int16_t speed = _motors.m1.direction() * lin_speed * cos(radians(move_angle + _loc_angles.m1_angle)) + angular_speed;
    _motors.m1.set_speed(speed);

    speed = _motors.m2.direction() * lin_speed * cos(radians(move_angle + _loc_angles.m2_angle)) + angular_speed;
    _motors.m2.set_speed(speed);

    speed = _motors.m3.direction() * lin_speed * cos(radians(move_angle + _loc_angles.m3_angle)) + angular_speed;
    _motors.m3.set_speed(speed);

    speed = _motors.m4.direction() * lin_speed * cos(radians(move_angle + _loc_angles.m4_angle)) + angular_speed;
    _motors.m4.set_speed(speed);
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
