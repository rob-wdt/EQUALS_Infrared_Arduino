#pragma once

#include "config.h"

#include "control/ball-detector/ball-detector.h"
#include "control/outs-handler-coords/outs-handler-coords.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"
#include "core/UART/UART.h"
#include "control/movement-controller/movement-controller.h"
#include "control/heading-controller/heading-controller.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "core/dribbler/dribbler.h"

#include "management/config/robot-config.h"
#include "core/kicker/kicker.h"
#include "core/led/led.h"
#include "management/state-machine/state-machine.h"

class Robot
{
public:
    // can_change_role: whether the robot may swap attacker / defender during the game (needs a teammate)
    Robot(RoleKind start_role, RobotType robot_type, GoalColor own_goal, bool can_change_role);
    void init();
    void calibrate_gyro();
    void listen();
    void calculate_coords();
    void update();
    State *current_state() { return _fsm.current(); }

private:
    RoleKind _start_role;
    bool _can_change_role;
    RobotType _robot_type;
    RobotConfig _config;

    BallDetector _ball_detector;
    OutsHandlerCoords _outs_handler;
    RobotCoordinatesCalculator _coords;
    CameraDataParser _cam_parser;
    MovementController _movement_controller;
    HeadingController _align;
    HeadingController _with_ball_align; // while the dribbler holds the ball
    BNO055 _gyro;
    Button _gyro_button;
    Button _start_button;
    Button _stop_button;
    Kicker _kicker;
    Led _led;
    Dribbler _dribbler;

    StateMachine _fsm;

    StateContext _make_context();
    void _apply(Command cmd);
};
