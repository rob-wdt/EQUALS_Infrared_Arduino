#include "robot-config.h"

RobotConfig::RobotConfig(RobotType robot) : _robot(robot)
{
    _speed = _robot.speed();
    _pins = _robot.pins();
    _uart_config = _robot.UART_config();
    _outs = _robot.outs_bounds();
    _motors_config = _robot.motors_config();
    _gyroscope_address = _robot.gyro_address();
    _controls_config = _robot.controls_config();
    _kicking_time_ms = _robot.kicking_time_ms();
    _phototransistor_threshold = _robot.phototransistor_threshold();
    _defender_move_coef = _robot.defender_move_coef();
}

controlsConfig RobotConfig::controls_config() { return _controls_config; }
int RobotConfig::defender_move_coef() { return _defender_move_coef; }
int RobotConfig::gyro_address() { return _gyroscope_address; }
int RobotConfig::kicking_time_ms() { return _kicking_time_ms; }
motorsConfig RobotConfig::motors_config() { return _motors_config; }
outsBounds RobotConfig::outs_bounds() { return _outs; }
int RobotConfig::phototransistor_threshold() { return _phototransistor_threshold; }
pin RobotConfig::pins() { return _pins; }
int RobotConfig::speed() { return _speed; }
UARTConfig RobotConfig::UART_config() { return _uart_config; }
