#include <Arduino.h>
#include "config.h"
#include "control/outs-handler-coords/outs-handler-coords.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"
#include "debug/debug-controls.h"

class OutsCommonTest
{
public:
    OutsCommonTest(OutsHandlerCoords outs, RobotCoordinatesCalculator coords, BNO055 gyro, DebugControls controls, UART uart, CameraDataParser cam_parser, Led led) : _outs(outs), _coords(coords), _gyro(gyro), _controls(controls), _uart(uart), _c(cam_parser), _led(led) {}
    void setup()
    {
        _gyro.init();
        _c.init();
        _controls.init();
        _led.init();
        Serial.begin(115200);

        _controls.calibrate_and_set_zero(_gyro, _led);
        _controls.wait_start();
    }
    void loop(outsBounds outs_bounds)
    {
        if (!_controls.update(_gyro, &_c))
            return;
        _gyro.read();
        _c.parse();
        _coords.calculate_coordinates(_gyro.data(), _c.data());

        point cur_coords = _coords.coordinates();

        _outs.handle(cur_coords, outs_bounds);

        String out_type;
        bool in_out;

        switch (_outs.out_type())
        {
        case OutType::LEFT:
            out_type = "LEFT";
            in_out = true;
            break;
        case OutType::FRONT_GOAL:
            out_type = "FRONT";
            in_out = true;
            break;
        case OutType::NO:
            out_type = "NO";
            in_out = false;
            break;
        case OutType::REAR_GOAL:
            out_type = "REAR";
            in_out = true;
            break;
        case OutType::RIGHT:
            out_type = "RIGHT";
            in_out = true;
            break;
        }

        Serial.print("In out: ");
        Serial.print(in_out);
        Serial.print("\tOut type: ");
        Serial.print(out_type);
        Serial.print("\t\t|\t");
        Serial.print("x: ");
        Serial.print(cur_coords.x);
        Serial.print("\ty: ");
        Serial.print(cur_coords.y);
        Serial.print("\t\t|\t");
        Serial.print("y <= rear_out: ");
        Serial.print(cur_coords.y <= outs_bounds.rear_goal);
        Serial.print("\trear out:\t");
        Serial.println(outs_bounds.rear_goal);
    }

private:
    OutsHandlerCoords _outs;
    RobotCoordinatesCalculator _coords;
    BNO055 _gyro;
    DebugControls _controls;
    UART _uart;
    CameraDataParser _c;
    Led _led;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

OutsHandlerCoords outs(config.outs_hysteresis());
RobotCoordinatesCalculator coords;
BNO055 gyro(config.gyro_address());
DebugControls controls(config.pins().btn1, config.pins().btn2, config.pins().btn3);
Led led(config.pins().led1);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalMapper goal(GoalColor::BLUE);
CameraDataParser cam_parser(uart, goal);

OutsCommonTest test(outs, coords, gyro, controls, uart, cam_parser, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.outs_bounds());
}
