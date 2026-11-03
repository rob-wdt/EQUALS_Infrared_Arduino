#pragma once

#include "config.h"
#include "core/UART/UART.h"

struct pin
{
    int phototransistor, btn1, btn2, btn3, m1_1, m1_2, m2_1, m2_2, m3_1, m3_2, m4_1, m4_2, solenoid, dribbler, led, distane_sensor;
};

struct UARTConfig
{
    UARTNum num;
    long int baudrate;
    int buffer_size, buffer_starter;
};

struct motorsConfig
{
    motorsLocAngles loc_angles;
    motorsDirections directions;
};

struct dribblerConfig
{
    int min, max;
};

struct controlsConfig
{
    PIDCoefficients pid;
    PDCCoefficients pdc;
    PDCCoefficients with_ball_pdc; // turning while the dribbler holds the ball
    int with_ball_max_speed;
};

class RobotType
{
public:
    RobotType();

    int speed();
    pin pins();
    UARTConfig UART_config();
    outsBounds outs_bounds();
    float outs_hysteresis();
    motorsConfig motors_config();
    int gyro_address();
    controlsConfig controls_config();
    int kicking_time_ms();
    dribblerConfig dribbler_config();
    int phototransistor_threshold();
    float ball_angle_offset();

protected:
    int _speed;
    pin _pins;
    UARTConfig _uart_config;
    outsBounds _outs;
    float _outs_hysteresis;
    motorsConfig _motors_config;
    int _gyroscope_address;
    controlsConfig _controls_config;
    int _kicking_time_ms;
    dribblerConfig _dribbler_config;
    int _phototransistor_threshold;
    float _ball_angle_offset;
};

// Robot types are distinguished by the color of the cube beneath the gyro

class RobotViolet : public RobotType
{
public:
    RobotViolet();
};

class RobotWhite : public RobotType
{
public:
    RobotWhite();
};
