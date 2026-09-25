#include <Arduino.h>
#include "config.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/sensors/button/button.h"
#include "core/UART/UART.h"
#include "control/camera-data-parcer/camera-data-parser.h"

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
        Serial.print(Pin::PIN_BTN_1);
        Serial.println(" to calibrate the gyroscope.");
        while (!_button.is_pressed())
            ;

        while (!_gyro.calibrated())
            ;

        Serial.println("Gyroscope calibrated");
        Serial.println("Press button #");
        Serial.print(Pin::PIN_BTN_1);
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

CoordinatesCalculator coords;
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);
UART uart(UARTNum::UART_3, 230400, UART_BUFFER_SIZE, UART_BUFFER_STARTER);
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
