#include <Arduino.h>
#include "config.h"
#include "control/outs-handler/outs-handler.h"
#include "control/movement-controller/movement-controller.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"

class OutsMovementTest
{
public:
    OutsMovementTest(OutsHandler outs, MovementController mc, CoordinatesCalculator coords, BNO055 gyro, Button button, UART uart, CameraDataParser cdp) : _outs(outs), _m(mc), _coords(coords), _gyro(gyro), _button(button), _uart(uart), _cam_parser(cdp) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
        _m.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to calibrate the gyroscope.");
        while (!_button.is_pressed())
            ;

        while (!_gyro.calibrated())
            ;

        Serial.println("Gyroscope calibrated");
        Serial.println("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to set the zero angles.");
        while (!_button.is_pressed())
            ;

        _gyro.set_zero_angles();
    }
    void loop(outsBounds outs_bounds, int speed)
    {
        _gyro.read();
        _cam_parser.parse();
        _coords.calculate_coordinates(_gyro.data(), _cam_parser.data().own_goal_angle, _cam_parser.data().own_goal_distance);

        _outs.handle(_coords.coordinates(), outs_bounds);

        _m.set_velocity(speed, _move_angle, 0, _gyro.data());
        _m.move();

        String out_type;
        bool in_out;

        switch (_outs.out_type())
        {
        case OutType::LEFT:
            out_type = "LEFT";
            in_out = true;
            _move_angle = 90;
            break;
        case OutType::FRONT_GOAL:
            out_type = "FRONT";
            in_out = true;
            _move_angle = 180;
            break;
        case OutType::NO:
            out_type = "NO";
            in_out = false;
            break;
        case OutType::REAR_GOAL:
            out_type = "REAR";
            in_out = true;
            _move_angle = 0;
            break;
        case OutType::RIGHT:
            out_type = "RIGHT";
            in_out = true;
            _move_angle = -90;
            break;
        }

        Serial.print("In out: ");
        Serial.print(in_out);
        Serial.print("\tOut type: ");
        Serial.println(out_type);
    }

private:
    OutsHandler _outs;
    MovementController _m;
    CoordinatesCalculator _coords;
    BNO055 _gyro;
    Button _button;
    UART _uart;
    CameraDataParser _cam_parser;

    int _move_angle = 0;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

OutsHandler outs;
MovementController movement_controller(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles,
    new PDCControl(config.controls_config().pdc));
CoordinatesCalculator coords;
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
CameraDataParser cam_parser(uart);

OutsMovementTest test(outs, movement_controller, coords, gyro, button, uart, cam_parser);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.outs_bounds(), config.speed());
}
