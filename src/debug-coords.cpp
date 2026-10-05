#include <Arduino.h>
#include "config.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/sensors/button/button.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"

class CoordsTest
{
public:
    CoordsTest(CoordinatesCalculator cc, BNO055 gyro, CameraDataParser cam_parser, Button button) : _c(cc), _gyro(gyro), _cam_parser(cam_parser), _button(button) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
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
    void loop()
    {
        _gyro.read();
        _cam_parser.parse();
        _c.calculate_coordinates(_gyro.data(), _cam_parser.data().own_goal_angle, _cam_parser.data().own_goal_distance);

        Serial.print("Current robot coords:\tx: ");
        Serial.print(_c.coordinates().x);
        Serial.print(" y: ");
        Serial.println(_c.coordinates().y);
    }

private:
    CoordinatesCalculator _c;
    BNO055 _gyro;
    CameraDataParser _cam_parser;
    Button _button;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

CoordinatesCalculator coords;
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
CameraDataParser cam_parser(uart);

CoordsTest test(coords, gyro, cam_parser, button);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
