#include <Arduino.h>
#include "config.h"
#include "control/outs-handler/outs-handler.h"
#include "control/movement-controller/movement-controller.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"

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
        _coords.calculate_coordinates(_gyro.data(), _cam_parser.data().own_goal_angle, _cam_parser.data().own_goal_distance);

        _outs.handle(_coords.coordinates(), OUTS_BOUNDS);

        _m.set_velocity(SPEED, _move_angle, 0, _gyro.data());
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

OutsHandler outs;
MovementController movement_controller(
    {Motor(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir),
     Motor(Pin::PIN_M2_1, Pin::PIN_M2_2, motors_directions.m2_dir),
     Motor(Pin::PIN_M3_1, Pin::PIN_M3_2, motors_directions.m3_dir),
     Motor(Pin::PIN_M4_1, Pin::PIN_M4_2, motors_directions.m4_dir)},
    motors_loc_angles,
    new PDCControl(control_gyro_align_pdc));
CoordinatesCalculator coords;
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);
UART uart(UARTNum::UART_1, 230400, UART_BUFFER_SIZE, UART_BUFFER_STARTER);
CameraDataParser cam_parser(uart);

OutsMovementTest test(outs, movement_controller, coords, gyro, button, uart, cam_parser);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
