#include <Arduino.h>
#include "config.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"
#include "debug/debug-controls.h"

class CoordsTest
{
public:
    CoordsTest(RobotCoordinatesCalculator cc, BNO055 gyro, CameraDataParser cam_parser, DebugControls controls, Led led) : _c(cc), _gyro(gyro), _cam_parser(cam_parser), _controls(controls), _led(led) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
        _controls.init();
        _led.init();
        Serial.begin(115200);

        _controls.calibrate_and_set_zero(_gyro, _led);
        _controls.wait_start();
    }
    void loop()
    {
        if (!_controls.update(_gyro, &_cam_parser))
            return;
        _gyro.read();
        _cam_parser.parse();
        _c.calculate_coordinates(_gyro.data(), _cam_parser.data());

        Serial.print("Current robot coords:\tx: ");
        Serial.print(_c.coordinates().x);
        Serial.print(" y: ");
        Serial.println(_c.coordinates().y);
    }

private:
    RobotCoordinatesCalculator _c;
    BNO055 _gyro;
    CameraDataParser _cam_parser;
    DebugControls _controls;
    Led _led;
};

RobotWhite robot_type;
RobotConfig config(robot_type);
GoalMapper goal(GoalColor::YELLOW);

RobotCoordinatesCalculator coords;
BNO055 gyro(config.gyro_address());
DebugControls controls(config.pins().btn1, config.pins().btn2, config.pins().btn3);
Led led(config.pins().led1);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
CameraDataParser cam_parser(uart, goal);

CoordsTest test(coords, gyro, cam_parser, controls, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
