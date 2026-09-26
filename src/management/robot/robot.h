#pragma once

#include "config.h"

#include "control/ball-detector/ball-detector.h"
#include "control/outs-handler/outs-handler.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/UART/UART.h"
#include "control/movement-controller/movement-controller.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "control/camera-data-parser/camera-data-parser.h"

#include "management/role/role.h"

class Robot
{
public:
    Robot(Role *initial_role, BallDetector bd, OutsHandler oh, CoordinatesCalculator coords, CameraDataParser cam_parser, MovementController mc, BNO055 gyro, Button gyro_button);
    void init();
    void calibrate_gyro();
    void listen();
    void calculate_coords();
    void update();
    void set_role(Role *role);

private:
    BallDetector _ball_detector;
    OutsHandler _outs_handler;
    CoordinatesCalculator _coords;
    CameraDataParser _cam_parser;
    MovementController _movement_controller;
    BNO055 _gyro;
    Button _gyro_button;

    Role *_role;
};
