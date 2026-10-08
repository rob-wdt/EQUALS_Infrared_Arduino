#pragma once

#include <stdint.h>

#include <Arduino.h>

#include "utils/control/PID-control/PID-control.h"
#include "utils/control/PDC-control/PDC-control.h"
#include "control/outs-handler/outs-handler.h"
#include "control/movement-controller/movement-controller.h"
#include "management/robot-type/robot-type.h"
#include "core/UART/UART.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------- VIOLET ------------------------------------------------------ //
#define SPEED_VIOLET 100

enum class PinViolet : uint8_t
{
    PIN_PHOTOTRANSISTOR = A14,
    PIN_BTN_1 = 36,
    PIN_BTN_2 = 32,
    PIN_BTN_3 = 34,
    PIN_M1_1 = 12,
    PIN_M1_2 = 10,
    PIN_M2_1 = 11,
    PIN_M2_2 = 9,
    PIN_M3_1 = 8,
    PIN_M3_2 = 6,
    PIN_M4_1 = 5,
    PIN_M4_2 = 7,
    PIN_SOLENOID = 31,
    PIN_DRIBBLER = 3
};

extern UARTNum UART_num_violet;
#define UART_BAUDRATE_VIOLET 230400
#define UART_BUFFER_SIZE_VIOLET 7
#define UART_BUFFER_STARTER_VIOLET 255

const outsBounds outs_bounds_violet = {60, -60, 50, -60};

const motorsLocAngles motors_loc_angles_violet = {
    MotorLocationAngle::ANGLE_315,
    MotorLocationAngle::ANGLE_45,
    MotorLocationAngle::ANGLE_135,
    MotorLocationAngle::ANGLE_225};

const motorsDirections motors_directions_violet = {
    MotorDirection::backward,
    MotorDirection::forward,
    MotorDirection::backward,
    MotorDirection::forward};

#define GYROSCOPE_ADDRESS_VIOLET 0x29

const PIDCoefficients control_gyro_align_pid_violet = {2.0, 0.0, 0.0};
const PDCCoefficients control_gyro_align_pdc_violet = {0.5, 0.05, 0.1};

#define KICKING_TIME_MS_VIOLET 200

#define DRIBBLER_MIN_VIOLET 1000
#define DRIBBLER_MAX_VIOLET 1500

#define PHOTOTRANSISTOR_THRESHOLD_VIOLET 1

#define DEFENDER_MOVE_COEF_VIOLET 0.5

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------- WHITE ------------------------------------------------------- //
#define SPEED_WHITE 100

enum class PinWhite : uint8_t
{
    PIN_PHOTOTRANSISTOR = A14,
    PIN_BTN_1 = 36,
    PIN_BTN_2 = 32,
    PIN_BTN_3 = 34,
    PIN_M1_1 = 12,
    PIN_M1_2 = 10,
    PIN_M2_1 = 11,
    PIN_M2_2 = 9,
    PIN_M3_1 = 8,
    PIN_M3_2 = 6,
    PIN_M4_1 = 5,
    PIN_M4_2 = 7,
    PIN_SOLENOID = 31,
    PIN_DRIBBLER = 3
};

extern UARTNum UART_num_white;
#define UART_BAUDRATE_WHITE 230400
#define UART_BUFFER_SIZE_WHITE 7
#define UART_BUFFER_STARTER_WHITE 255

const outsBounds outs_bounds_white = {0, 0, 0, 0};

const motorsLocAngles motors_loc_angles_white = {
    MotorLocationAngle::ANGLE_315,
    MotorLocationAngle::ANGLE_45,
    MotorLocationAngle::ANGLE_135,
    MotorLocationAngle::ANGLE_225};

const motorsDirections motors_directions_white = {
    MotorDirection::backward,
    MotorDirection::forward,
    MotorDirection::backward,
    MotorDirection::forward};

#define GYROSCOPE_ADDRESS_WHITE 0x29

const PIDCoefficients control_gyro_align_pid_white = {2.0, 0.0, 0.0};
const PDCCoefficients control_gyro_align_pdc_white = {0.5, 0.05, 0.1};

#define KICKING_TIME_MS_WHITE 200

#define DRIBBLER_MIN_WHITE 1000
#define DRIBBLER_MAX_WHITE 1500

#define PHOTOTRANSISTOR_THRESHOLD_WHITE 1

#define DEFENDER_MOVE_COEF_WHITE 0.5
