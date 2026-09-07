#pragma once

#include <stdint.h>

#include <Arduino.h>

#include "utils/control/PID-control/PID-control.h"
#include "utils/control/PDC-control/PDC-control.h"
#include "control/outs-handler/outs-handler.h"
#include "control/movement-controller/movement-controller.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------- GENERAL ----------------------------------------------------- //
#define SPEED 100

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------- DEBUG ----------------------------------------------------- //
#define DEBUG_MODE_NONE 0
#define DEBUG_MODE_PHOTOTRANSISTOR 1
#define DEBUG_MODE_BUTTON 2
#define DEBUG_MODE_MOTORS 3
#define DEBUG_MODE_GYROSCOPE 4
#define DEBUG_MODE_PID 5
#define DEBUG_MODE_PDC 6
#define DEBUG_MODE_MOVEMENT_CONTROLLER 7
#define DEBUG_MODE_BALL_DETECTOR 8
#define DEBUG_MODE_COORDS 9
#define DEBUG_MODE_CAM_PERCEPTION 10
#define DEBUG_MODE_CAM_DATA_PARSER 11
#define DEBUG_MODE_OUTS_COMMON 12
#define DEBUG_MODE_OUTS_MOVEMENT 13

#define DEBUG_MODE DEBUG_MODE_PID

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------- PINS ------------------------------------------------------ //
enum Pin : uint8_t
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
    PIN_M4_2 = 7
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------- UART ------------------------------------------------------ //
#define UART_BUFFER_SIZE 7
#define UART_BUFFER_STARTER 255

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------- OUTS ------------------------------------------------------ //
const outsBounds OUTS_BOUNDS = {0, 0, 0, 0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------- Motors ------------------------------------------------------ //

const motorsLocAngles motors_loc_angles = {
    MotorLocationAngle::ANGLE_45,
    MotorLocationAngle::ANGLE_135,
    MotorLocationAngle::ANGLE_225,
    MotorLocationAngle::ANGLE_315};

const motorsDirections motors_directions = {
    MotorDirection::forward,
    MotorDirection::forward,
    MotorDirection::forward,
    MotorDirection::forward};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- GYROSCOPE ----------------------------------------------------- //
#define GYROSCOPE_ADDRESS 0x29

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- CONTROLS ------------------------------------------------------ //
const PIDCoefficients control_gyro_align_pid = {2.0, 0.0, 0.0};
const PDCCoefficients control_gyro_align_pdc = {0.5, 0.05, 0.1};
