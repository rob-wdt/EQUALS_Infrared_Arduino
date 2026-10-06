#pragma once

#include "config.h"
#include "core/UART/UART.h"

struct pin
{
    int phototransistor, btn1, btn2, btn3, m1_1, m1_2, m2_1, m2_2, m3_1, m3_2, m4_1, m4_2, solenoid, dribbler;
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
};

class RobotType
{
public:
    RobotType();

    int speed();
    pin pins();
    UARTConfig UART_config();
    outsBounds outs_bounds();
    motorsConfig motors_config();
    int gyro_address();
    controlsConfig controls_config();
    int kicking_time_ms();
    dribblerConfig dribbler_config();
    int phototransistor_threshold();
    int defender_move_coef();

protected:
    int _speed;
    pin _pins;
    UARTConfig _uart_config;
    outsBounds _outs;
    motorsConfig _motors_config;
    int _gyroscope_address;
    controlsConfig _controls_config;
    int _kicking_time_ms;
    dribblerConfig _dribbler_config;
    int _phototransistor_threshold;
    int _defender_move_coef;
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
