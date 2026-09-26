#include "robot.h"

Robot::Robot(Role *initial_role,
             BallDetector bd,
             OutsHandler oh,
             CoordinatesCalculator coords,
             CameraDataParser cam_parser,
             MovementController mc,
             BNO055 gyro,
             Button gyro_button) : _role(initial_role),
                                   _ball_detector(bd),
                                   _outs_handler(oh),
                                   _coords(coords),
                                   _cam_parser(cam_parser),
                                   _movement_controller(mc),
                                   _gyro(gyro),
                                   _gyro_button(gyro_button) {}

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
    Serial.print(Pin::PIN_BTN_1);
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
    Serial.print(Pin::PIN_BTN_1);
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
}

void Robot::update()
{
    calculate_coords();
    _outs_handler.handle(_coords.coordinates(), OUTS_BOUNDS);

    _role->update(_cam_parser.data());

    switch (_outs_handler.out_type())
    {
    case OutType::NO:
        _movement_controller.set_velocity(_role->lin_speed(), _role->move_angle(), _role->align_angle(), _gyro.data());
        _movement_controller.move();
        break;
    case OutType::LEFT:
        _movement_controller.set_velocity(SPEED, 90, _role->align_angle(), _gyro.data());
        break;
    case OutType::RIGHT:
        _movement_controller.set_velocity(SPEED, -90, _role->align_angle(), _gyro.data());
        break;
    case OutType::FRONT_GOAL:
        _movement_controller.set_velocity(SPEED, 180, _role->align_angle(), _gyro.data());
        break;
    case OutType::REAR_GOAL:
        _movement_controller.set_velocity(SPEED, 0, _role->align_angle(), _gyro.data());
        break;
    }
}

void Robot::set_role(Role *role)
{
    _role = role;
}
