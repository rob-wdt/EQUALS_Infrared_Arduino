#include "robot.h"

Robot::Robot(Role *initial_role, RobotType robot_type) : _role(initial_role),
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
                                                         _gyro(_config.gyro_address()), _gyro_button(_config.pins().btn1) {}

void Robot::init()
{
    _movement_controller.init();
    _cam_parser.init();
    _movement_controller.init();
    _gyro.init();
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
}

void Robot::calculate_coords()
{
    _coords.calculate_coordinates(_gyro.data(), _cam_parser.data().own_goal_angle, _cam_parser.data().own_goal_distance);
    _outs_handler.handle(_coords.coordinates(), _config.outs_bounds());
}

void Robot::update()
{
    listen();
    calculate_coords();

    _role->update(_cam_parser.data());

    switch (_outs_handler.out_type())
    {
    case OutType::NO:
        _movement_controller.set_velocity(_role->lin_speed(), _role->move_angle(), _role->align_angle(), _gyro.data());
        _movement_controller.move();
        break;
    case OutType::LEFT:
        _movement_controller.set_velocity(_config.speed(), 90, _role->align_angle(), _gyro.data());
        break;
    case OutType::RIGHT:
        _movement_controller.set_velocity(_config.speed(), -90, _role->align_angle(), _gyro.data());
        break;
    case OutType::FRONT_GOAL:
        _movement_controller.set_velocity(_config.speed(), 180, _role->align_angle(), _gyro.data());
        break;
    case OutType::REAR_GOAL:
        _movement_controller.set_velocity(_config.speed(), 0, _role->align_angle(), _gyro.data());
        break;
    }
}

void Robot::set_role(Role *role)
{
    _role = role;
}
