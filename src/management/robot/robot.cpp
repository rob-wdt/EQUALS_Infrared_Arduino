#include "robot.h"

Robot::Robot(RoleKind start_role, RobotType robot_type, GoalColor own_goal, bool can_change_role) : _start_role(start_role),
                                                                                                    _can_change_role(can_change_role),
                                                                                                    _robot_type(robot_type),
                                                                                                    _config(_robot_type),
                                                                                                    _ball_detector(Phototransistor(_config.pins().phototransistor), _config.phototransistor_threshold()),
                                                                                                    _outs_handler(_config.outs_hysteresis()),
                                                                                                    _cam_parser(UART(_config.UART_config().num, _config.UART_config().baudrate, _config.UART_config().buffer_size, _config.UART_config().buffer_starter), GoalMapper(own_goal)),
                                                                                                    _movement_controller(motors{
                                                                                                                             Motor(_config.pins().m1_1, _config.pins().m1_2, _config.motors_config().directions.m1_dir),
                                                                                                                             Motor(_config.pins().m2_1, _config.pins().m2_2, _config.motors_config().directions.m2_dir),
                                                                                                                             Motor(_config.pins().m3_1, _config.pins().m3_2, _config.motors_config().directions.m3_dir),
                                                                                                                             Motor(_config.pins().m4_1, _config.pins().m4_2, _config.motors_config().directions.m4_dir)},
                                                                                                                         _config.motors_config().loc_angles),
                                                                                                    _align(new PDCControl(_config.controls_config().pdc)), _with_ball_align(new PDCControl(_config.controls_config().with_ball_pdc), _config.controls_config().with_ball_max_speed), _gyro(_config.gyro_address()), _start_stop_button(_config.pins().btn1), _gyro_button(_config.pins().btn2), _goal_button(_config.pins().btn3), _kicker(_config.pins().solenoid, _config.kicking_time_ms()), _led(_config.pins().led1), _dribbler(_config.pins().dribbler, _config.dribbler_config().min, _config.dribbler_config().max), _state_indicator(Led(_config.pins().led1), Led(_config.pins().led2), Led(_config.pins().led3)), _rcmodule(_config.pins().robocup_module) {}

void Robot::init()
{
    _movement_controller.init();
    _cam_parser.init();
    _gyro.init();
    _start_stop_button.init();
    _gyro_button.init();
    _goal_button.init();
    _kicker.init();
    _led.init();
    _dribbler.init();
    _state_indicator.init();
    _rcmodule.init();
}

void Robot::calibrate_gyro()
{
    StateContext ctx = _make_context();
    Command cmd;
    _fsm.update(ctx, cmd);

    Serial.begin(115200);

    Serial.print("Press button #");
    Serial.print(_gyro_button.pin());
    Serial.println(" to calibrate the gyroscope and set zero angles.");
    _gyro_button.wait_click();

    while (!_gyro.calibrated())
        ;
    _led.on();

    Serial.println("Gyroscope calibrated");
    _gyro.set_zero_angles();

    Serial.print("Press button #");
    Serial.print(_start_stop_button.pin());
    Serial.println(" to start.");
    _start_stop_button.wait_click();
    _start_requested = true;
}

void Robot::listen()
{
    _ball_detector.read();
    _gyro.read();
    _start_stop_button.read();
    _gyro_button.read();
    _goal_button.read();

    if (_start_stop_button.was_pressed())
    {
        if (_fsm.current() == &_fsm.states().idle)
            _start_requested = true;
        else
            _stop_requested = true;
    }

    if (_gyro_button.was_pressed())
    {
        _recalibrate_requested = true;
        _stop_requested = true;
    }

    if (_goal_button.was_pressed())
    {
        _cam_parser.toggle_own_goal();
        _fsm.reset_field_reference();
        Serial.print("Own goal: ");
        Serial.println(_cam_parser.own_goal() == GoalColor::YELLOW ? "YELLOW" : "BLUE");
    }

    _cam_parser.parse();
    _rcmodule.read();

    bool rc_start = _rcmodule.start();
    if (rc_start && !_previous_rc_start)
        _start_requested = true;
    _previous_rc_start = rc_start;
}

void Robot::calculate_coords()
{
    _coords.calculate_coordinates(_gyro.data(), _cam_parser.data());
    _outs_handler.handle(_coords.coordinates(), _config.outs_bounds());
}

StateContext Robot::_make_context()
{
    StateContext ctx = {};
    ctx.cam = _cam_parser.data();
    // The camera sees the ball a little off (the robot misses it to one side): correct it once here,
    // so the states, the ball coordinates and the alignment all get the same angle. Not when the ball isn't seen (0)
    if (ctx.cam.ball_distance > 0)
        ctx.cam.ball_angle -= _config.ball_angle_offset();
    ctx.gyro = _gyro.data();
    ctx.coords = _coords.coordinates();
    ctx.out = _outs_handler.out_type();
    ctx.out_escape_angle = _outs_handler.escape_angle();
    ctx.ball_visible = ctx.cam.ball_distance > 0;
    ctx.own_goal_visible = ctx.cam.own_goal_distance > 0;
    ctx.ball_captured = _ball_detector.ball_detected();
    ctx.stop_pressed = _stop_requested || !_rcmodule.start();
    ctx.start_pressed = !ctx.stop_pressed && _start_requested;
    _start_requested = false;
    _stop_requested = false;
    ctx.start_role = _start_role;
    ctx.params.speed = _config.speed();
    return ctx;
}

void Robot::_apply(Command cmd)
{
    if (cmd.calibrate)
        calibrate_gyro();

    if (cmd.kick)
        _kicker.kick();
    _kicker.unkick();

    if (cmd.stop)
    {
        _movement_controller.stop();
        return;
    }

    if (cmd.dribble)
        _dribbler.run();
    else
        _dribbler.stop();

    // Both regulators run every tick, so the D term of the other one is up to date when the mode switches
    float align_speed = _align.calculate(cmd.align_angle, _gyro.data());
    float with_ball_align_speed = _with_ball_align.calculate(cmd.align_angle, _gyro.data());
    float angular_speed = cmd.align_mode == AlignMode::BALL_HOLD ? with_ball_align_speed : align_speed;

    _movement_controller.set_velocity(cmd.lin_speed, cmd.move_angle, angular_speed);
    _movement_controller.move();
}

void Robot::update()
{
    listen();

    if (_recalibrate_requested)
    {
        _movement_controller.stop();
        while (!_gyro.calibrated())
            ;
        _gyro.set_zero_angles();
        _recalibrate_requested = false;
        Serial.println("Gyroscope calibrated; zero angles set");
    }

    calculate_coords();

    StateContext ctx = _make_context();
    Command cmd;
    _fsm.update(ctx, cmd);
    _apply(cmd);

    _state_indicator.indicate(*_fsm.current(), ctx);
}
