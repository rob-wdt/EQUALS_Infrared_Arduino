#pragma once

#include "management/robot-type/robot-type.h"

class RobotConfig
{
public:
    RobotConfig(RobotType robot);

    int speed();
    pin pins();
    outsBounds outs_bounds();
    motorsConfig motors_config();
    int gyro_address();
    controlsConfig controls_config();
    int kicking_time_ms();
    dribblerConfig dribbler_config();
    int phototransistor_threshold();
    int defender_move_coef();
    UARTConfig UART_config();

protected:
    RobotType _robot;

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
