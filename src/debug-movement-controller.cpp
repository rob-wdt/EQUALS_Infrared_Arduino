#include <Arduino.h>
#include "config.h"
#include "control/movement-controller/movement-controller.h"
#include "core/sensors/button/button.h"

// FEEL FREE TO CHANGE
#define MOVE_ANGLE 0
#define ALIGN_ANGLE 0

class MovementControllerTest
{
public:
    MovementControllerTest(MovementController mc, BNO055 gyro, Button btn) : _m(mc), _gyro(gyro), _button(button) {}
    void setup()
    {
        _gyro.init();
        _m.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(Pin::PIN_BTN_1);
        Serial.println(" to calibrate the gyroscope.");
        _button.reset();
        while (!_button.is_pressed())
        {
            _button.read();
        }

        while (!_gyro.calibrated())
            ;

        Serial.println("Gyroscope calibrated");
        Serial.print("Press button #");
        Serial.print(Pin::PIN_BTN_1);
        Serial.println(" to set the zero angles.");
        while (!_button.is_pressed())
        {
            _button.read();
        }

        _gyro.set_zero_angles();

        Serial.println("Zero angles set.");
    }
    void loop()
    {
        _gyro.read();
        gyroData data = _gyro.data();

        _m.set_velocity(SPEED, MOVE_ANGLE, ALIGN_ANGLE, data);
        _m.move();
    }

private:
    MovementController _m;
    BNO055 _gyro;
    Button _button;
};

MovementController movement_controller(
    {Motor(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir),
     Motor(Pin::PIN_M2_1, Pin::PIN_M2_2, motors_directions.m2_dir),
     Motor(Pin::PIN_M3_1, Pin::PIN_M3_2, motors_directions.m3_dir),
     Motor(Pin::PIN_M4_1, Pin::PIN_M4_2, motors_directions.m4_dir)},
    motors_loc_angles,
    new PIDControl(control_gyro_align_pid));

BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

MovementControllerTest test(movement_controller, gyro, button);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
