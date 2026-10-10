#include <Arduino.h>
#include "config.h"
#include "control/outs-handler/outs-handler.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"

class OutsCommonTest
{
public:
    OutsCommonTest(OutsHandler outs, CoordinatesCalculator coords, BNO055 gyro, Button button, UART uart, CameraDataParser cam_parser, Led led) : _outs(outs), _coords(coords), _gyro(gyro), _button(button), _uart(uart), _c(cam_parser), _led(led) {}
    void setup()
    {
        _gyro.init();
        _c.init();
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
    void loop(outsBounds outs_bounds)
    {
        _gyro.read();
        _c.parse();
        _coords.calculate_coordinates(_gyro.data(), _c.data());

        coordinates cur_coords = _coords.coordinates();

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
    OutsHandler _outs;
    CoordinatesCalculator _coords;
    BNO055 _gyro;
    Button _button;
    Led _led;
    UART _uart;
    CameraDataParser _c;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

OutsHandler outs(config.outs_hysteresis());
CoordinatesCalculator coords;
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
Led led(config.pins().led);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalMapper goal(GoalColor::BLUE);
CameraDataParser cam_parser(uart, goal);

OutsCommonTest test(outs, coords, gyro, button, uart, cam_parser, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.outs_bounds());
}
