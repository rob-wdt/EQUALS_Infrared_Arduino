#include "robot.h"

Robot::Robot(RoleKind start_role, RobotType robot_type) : _start_role(start_role),
                                                          _robot_type(robot_type),
                                                          _config(_robot_type),
                                                          _ball_detector(Phototransistor(_config.pins().phototransistor), _config.phototransistor_threshold()),
                                                          _cam_parser(UART(_config.UART_config().num, _config.UART_config().baudrate, _config.UART_config().buffer_size, _config.UART_config().buffer_starter)),
                                                          _movement_controller(motors{
                                                                                   Motor(_config.pins().m1_1, _config.pins().m1_2, _config.motors_config().directions.m1_dir),
                                                                                   Motor(_config.pins().m2_1, _config.pins().m2_2, _config.motors_config().directions.m2_dir),
                                                                                   Motor(_config.pins().m3_1, _config.pins().m3_2, _config.motors_config().directions.m3_dir),
                                                                                   Motor(_config.pins().m4_1, _config.pins().m4_2, _config.motors_config().directions.m4_dir)},
                                                                               _config.motors_config().loc_angles, new PDCControl(_config.controls_config().pdc)),
                                                          _gyro(_config.gyro_address()), _gyro_button(_config.pins().btn1), _start_button(_config.pins().btn2), _stop_button(_config.pins().btn3), _kicker(_config.pins().solenoid, _config.kicking_time_ms()) {}

void Robot::init()
{
    _movement_controller.init();
    _cam_parser.init();
    _gyro.init();
    _start_button.init();
    _stop_button.init();
    _kicker.init();
}

void Robot::calibrate_gyro()
{
    _gyro.init();
    Serial.begin(115200);

    Serial.print("Press button #");
    Serial.print(_config.pins().btn1);
    Serial.println(" to calibrate the gyroscope.");
    _gyro_button.reset();
    while (!_gyro_button.is_pressed())
    {
        _gyro_button.read();
    }

    while (!_gyro.calibrated())
        ;

    Serial.println("Gyroscope calibrated");
    Serial.println("Press button #");
    Serial.print(_config.pins().btn1);
    Serial.println(" to set the zero angles.");
    while (!_gyro_button.is_pressed())
        ;

    _gyro.set_zero_angles();
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
    _coords.calculate_coordinates(_gyro.data(), _cam_parser.data().own_goal_angle, _cam_parser.data().own_goal_distance);
    _outs_handler.handle(_coords.coordinates(), _config.outs_bounds());
}

StateContext Robot::_make_context()
{
    StateContext ctx = {};
    ctx.cam = _cam_parser.data();
    ctx.gyro = _gyro.data();
    ctx.coords = _coords.coordinates();
    ctx.out = _outs_handler.out_type();
    ctx.ball_visible = ctx.cam.ball_distance > 0;
    ctx.ball_captured = _ball_detector.ball_detected();
    ctx.start_pressed = _start_button.is_pressed();
    ctx.stop_pressed = _stop_button.is_pressed();
    ctx.start_role = _start_role;
    ctx.params.speed = _config.speed();
    ctx.params.defender_coef = _config.defender_move_coef();
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
    _movement_controller.set_velocity(cmd.lin_speed, cmd.move_angle, cmd.align_angle, _gyro.data());
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
