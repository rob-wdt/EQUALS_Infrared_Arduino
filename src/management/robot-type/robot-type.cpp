#include "robot-type.h"

RobotType::RobotType() {}

controlsConfig RobotType::controls_config() { return _controls_config; }
dribblerConfig RobotType::dribbler_config() { return _dribbler_config; }
int RobotType::gyro_address() { return _gyroscope_address; }
int RobotType::kicking_time_ms() { return _kicking_time_ms; }
motorsConfig RobotType::motors_config() { return _motors_config; }
outsBounds RobotType::outs_bounds() { return _outs; }
float RobotType::outs_hysteresis() { return _outs_hysteresis; }
int RobotType::phototransistor_threshold() { return _phototransistor_threshold; }
float RobotType::ball_angle_offset() { return _ball_angle_offset; }
pin RobotType::pins() { return _pins; }
int RobotType::speed() { return _speed; }
UARTConfig RobotType::UART_config() { return _uart_config; }

pin pins_violet = {
    static_cast<int>(PinViolet::PIN_PHOTOTRANSISTOR),
    static_cast<int>(PinViolet::PIN_BTN_1),
    static_cast<int>(PinViolet::PIN_BTN_2),
    static_cast<int>(PinViolet::PIN_BTN_3),
    static_cast<int>(PinViolet::PIN_M1_1),
    static_cast<int>(PinViolet::PIN_M1_2),
    static_cast<int>(PinViolet::PIN_M2_1),
    static_cast<int>(PinViolet::PIN_M2_2),
    static_cast<int>(PinViolet::PIN_M3_1),
    static_cast<int>(PinViolet::PIN_M3_2),
    static_cast<int>(PinViolet::PIN_M4_1),
    static_cast<int>(PinViolet::PIN_M4_2),
    static_cast<int>(PinViolet::PIN_SOLENOID),
    static_cast<int>(PinViolet::PIN_DRIBBLER),
    static_cast<int>(PinViolet::PIN_LED),
    static_cast<int>(PinViolet::PIN_DISTANCE_SENSOR)};

UARTConfig uart_config_violet = {UART_num_violet, UART_BAUDRATE_VIOLET, UART_BUFFER_SIZE_VIOLET, UART_BUFFER_STARTER_VIOLET};

outsBounds outs_bounds_violet = {
    static_cast<float>(OutsBoundsViolet::RIGHT),
    static_cast<float>(OutsBoundsViolet::LEFT),
    static_cast<float>(OutsBoundsViolet::FRONT_GOAL),
    static_cast<float>(OutsBoundsViolet::REAR_GOAL),
    static_cast<float>(OutsBoundsViolet::FRONT_CORNERS),
    static_cast<float>(OutsBoundsViolet::REAR_CORNERS),
    static_cast<float>(OutsBoundsViolet::GOAL_ZONE_HALF_WIDTH)};

motorsConfig motors_config_violet = {motors_loc_angles_violet, motors_directions_violet};

controlsConfig controls_config_violet = {control_gyro_align_pid_violet, control_gyro_align_pdc_violet, control_with_ball_align_pdc_violet, WITH_BALL_ALIGN_MAX_SPEED_VIOLET};

dribblerConfig dribbler_config_violet = {DRIBBLER_MIN_VIOLET, DRIBBLER_MAX_VIOLET};

RobotViolet::RobotViolet() : RobotType()
{
    _speed = SPEED_VIOLET;
    _pins = pins_violet;
    _uart_config = uart_config_violet;
    _outs = outs_bounds_violet;
    _outs_hysteresis = OUTS_HYSTERESIS_VIOLET;
    _motors_config = motors_config_violet;
    _gyroscope_address = GYROSCOPE_ADDRESS_VIOLET;
    _controls_config = controls_config_violet;
    _kicking_time_ms = KICKING_TIME_MS_VIOLET;
    _dribbler_config = dribbler_config_violet;
    _phototransistor_threshold = PHOTOTRANSISTOR_THRESHOLD_VIOLET;
    _ball_angle_offset = BALL_ANGLE_OFFSET_VIOLET;
}

pin pins_white = {
    static_cast<int>(PinWhite::PIN_PHOTOTRANSISTOR),
    static_cast<int>(PinWhite::PIN_BTN_1),
    static_cast<int>(PinWhite::PIN_BTN_2),
    static_cast<int>(PinWhite::PIN_BTN_3),
    static_cast<int>(PinWhite::PIN_M1_1),
    static_cast<int>(PinWhite::PIN_M1_2),
    static_cast<int>(PinWhite::PIN_M2_1),
    static_cast<int>(PinWhite::PIN_M2_2),
    static_cast<int>(PinWhite::PIN_M3_1),
    static_cast<int>(PinWhite::PIN_M3_2),
    static_cast<int>(PinWhite::PIN_M4_1),
    static_cast<int>(PinWhite::PIN_M4_2),
    static_cast<int>(PinWhite::PIN_SOLENOID),
    static_cast<int>(PinWhite::PIN_DRIBBLER),
    static_cast<int>(PinWhite::PIN_LED),
    static_cast<int>(PinWhite::PIN_DISTANCE_SENSOR)};

UARTConfig uart_config_white = {UART_num_white, UART_BAUDRATE_WHITE, UART_BUFFER_SIZE_WHITE, UART_BUFFER_STARTER_WHITE};

outsBounds outs_bounds_white = {
    static_cast<float>(OutsBoundsWhite::RIGHT),
    static_cast<float>(OutsBoundsWhite::LEFT),
    static_cast<float>(OutsBoundsWhite::FRONT_GOAL),
    static_cast<float>(OutsBoundsWhite::REAR_GOAL),
    static_cast<float>(OutsBoundsWhite::FRONT_CORNERS),
    static_cast<float>(OutsBoundsWhite::REAR_CORNERS),
    static_cast<float>(OutsBoundsWhite::GOAL_ZONE_HALF_WIDTH)};

motorsConfig motors_config_white = {motors_loc_angles_white, motors_directions_white};

controlsConfig controls_config_white = {control_gyro_align_pid_white, control_gyro_align_pdc_white, control_with_ball_align_pdc_white, WITH_BALL_ALIGN_MAX_SPEED_WHITE};

dribblerConfig dribbler_config_white = {DRIBBLER_MIN_WHITE, DRIBBLER_MAX_WHITE};

RobotWhite::RobotWhite() : RobotType()
{
    _speed = SPEED_WHITE;
    _pins = pins_white;
    _uart_config = uart_config_white;
    _outs = outs_bounds_white;
    _outs_hysteresis = OUTS_HYSTERESIS_WHITE;
    _motors_config = motors_config_white;
    _gyroscope_address = GYROSCOPE_ADDRESS_WHITE;
    _controls_config = controls_config_white;
    _kicking_time_ms = KICKING_TIME_MS_WHITE;
    _dribbler_config = dribbler_config_white;
    _phototransistor_threshold = PHOTOTRANSISTOR_THRESHOLD_WHITE;
    _ball_angle_offset = BALL_ANGLE_OFFSET_WHITE;
}
