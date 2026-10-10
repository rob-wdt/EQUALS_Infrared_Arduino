#include <Arduino.h>
#include "config.h"
#include "control/outs-handler/outs-handler.h"
#include "control/movement-controller/movement-controller.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"
#include "management/goal-mapper/goal-mapper.h"

class OutsMovementTest
{
public:
    OutsMovementTest(OutsHandler outs, MovementController mc, CoordinatesCalculator coords, BNO055 gyro, Button button, UART uart, CameraDataParser cdp, Led led) : _outs(outs), _m(mc), _coords(coords), _gyro(gyro), _button(button), _uart(uart), _cam_parser(cdp), _led(led) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
        _m.init();
        _button.init();
        _led.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to calibrate the gyroscope.");
        _button.wait_click();

        while (!_gyro.calibrated())
            ;
        _led.on();

        Serial.println("Gyroscope calibrated");
        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to set the zero angles.");
        _button.wait_click();
        _gyro.set_zero_angles();

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to start.");
        _button.wait_click();
    }
    void loop(outsBounds outs_bounds, int speed)
    {
        _gyro.read();
        _cam_parser.parse();
        _coords.calculate_coordinates(_gyro.data(), _cam_parser.data());

        _outs.handle(_coords.coordinates(), outs_bounds);

        String out_type;
        bool in_out = _outs.in_out();
        _move_angle = _outs.escape_angle();
        Serial.print("Move angle: ");
        Serial.println(_move_angle);

        switch (_outs.out_type())
        {
        case OutType::LEFT:
            out_type = "LEFT";
            break;
        case OutType::FRONT_GOAL:
            out_type = "FRONT";
            break;
        case OutType::NO:
            out_type = "NO";
            break;
        case OutType::REAR_GOAL:
            out_type = "REAR";
            break;
        case OutType::RIGHT:
            out_type = "RIGHT";
            break;
        }

        // The escape angle is in field coordinates, set_velocity wants it relative to the robot
        _m.set_velocity(speed, _move_angle - _gyro.data().yaw, 0, _gyro.data());
        _m.move();

        Serial.print("In out: ");
        Serial.print(in_out);
        Serial.print("\tOut type: ");
        Serial.print(out_type);
        Serial.print("\tMove angle: ");
        Serial.print(_move_angle);
        Serial.print("\t\t|\t");
        Serial.print("x: ");
        Serial.print(_coords.coordinates().x);
        Serial.print("\ty: ");
        Serial.println(_coords.coordinates().y);
    }

private:
    OutsHandler _outs;
    MovementController _m;
    CoordinatesCalculator _coords;
    BNO055 _gyro;
    Button _button;
    Led _led;
    UART _uart;
    CameraDataParser _cam_parser;

    int _move_angle = 0;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

OutsHandler outs(config.outs_hysteresis());
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
Led led(config.pins().led);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalColor own_goal = GoalColor::BLUE;
CameraDataParser cam_parser(uart, GoalMapper(own_goal));

OutsMovementTest test(outs, movement_controller, coords, gyro, button, uart, cam_parser, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.outs_bounds(), config.speed());
}
