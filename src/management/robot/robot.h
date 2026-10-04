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

#include "management/config/robot-config.h"
#include "core/kicker/kicker.h"
#include "management/state-machine/state-machine.h"

class Robot
{
public:
    Robot(RoleKind start_role, RobotType robot_type);
    void init();
    void calibrate_gyro();
    void listen();
    void calculate_coords();
    void update();

private:
    // _config must be declared first: the members below are built from it
    RobotType _robot_type;
    RobotConfig _config;

    BallDetector _ball_detector;
    OutsHandler _outs_handler;
    CoordinatesCalculator _coords;
    CameraDataParser _cam_parser;
    MovementController _movement_controller;
    BNO055 _gyro;
    Button _gyro_button;
    Button _start_button;
    Button _stop_button;
    Kicker _kicker;

    StateMachine _fsm;
    RoleKind _start_role;

    StateContext _make_context();
    void _apply(Command cmd);
};
