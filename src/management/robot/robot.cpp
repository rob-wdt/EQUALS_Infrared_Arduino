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
                                                                                                    _align(new PDCControl(_config.controls_config().pdc)), _with_ball_align(new PDCControl(_config.controls_config().with_ball_pdc), _config.controls_config().with_ball_max_speed), _gyro(_config.gyro_address()), _gyro_button(_config.pins().btn1), _start_button(_config.pins().btn2), _stop_button(_config.pins().btn3), _kicker(_config.pins().solenoid, _config.kicking_time_ms()), _led(_config.pins().led), _dribbler(_config.pins().dribbler, _config.dribbler_config().min, _config.dribbler_config().max) {}

void Robot::init()
{
    _movement_controller.init();
    _cam_parser.init();
    _gyro.init();
    _gyro_button.init();
    _start_button.init();
    _stop_button.init();
    _kicker.init();
    _led.init();
    _dribbler.init();
}

void Robot::calibrate_gyro()
{
    StateContext ctx = _make_context();
    Command cmd;
    _fsm.update(ctx, cmd);

    Serial.begin(115200);

    Serial.print("Press button #");
    Serial.print(_gyro_button.pin());
    Serial.println(" to calibrate the gyroscope.");
    _gyro_button.wait_click();

    while (!_gyro.calibrated())
        ;
    _led.on();

    Serial.println("Gyroscope calibrated");
    Serial.print("Press button #");
    Serial.print(_gyro_button.pin());
    Serial.println(" to set the zero angles.");
    _gyro_button.wait_click();
    _gyro.set_zero_angles();

    Serial.print("Press button #");
    Serial.print(_gyro_button.pin());
    Serial.println(" to start.");
    _gyro_button.wait_click();
}

void Robot::listen()
{
    _ball_detector.read();
    _gyro.read();
    _cam_parser.parse();
    _start_button.read();
    _stop_button.read();
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
    ctx.ball_captured = _ball_detector.ball_detected();
    ctx.start_pressed = _start_button.is_pressed();
    ctx.stop_pressed = _stop_button.is_pressed();
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
    calculate_coords();

    StateContext ctx = _make_context();
    Command cmd;
    _fsm.update(ctx, cmd);
    _apply(cmd);
}
