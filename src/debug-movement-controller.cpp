#include <Arduino.h>
#include "config.h"
#include "control/movement-controller/movement-controller.h"
#include "core/sensors/button/button.h"
#include "management/config/robot-config.h"

// FEEL FREE TO CHANGE
#define MOVE_ANGLE 0
#define ALIGN_ANGLE 0

class MovementControllerTest
{
public:
    MovementControllerTest(MovementController mc, BNO055 gyro, Button btn) : _m(mc), _gyro(gyro), _button(btn) {}
    void setup()
    {
        _gyro.init();
        _m.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(_button.pin());
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
        Serial.print(_button.pin());
        Serial.println(" to set the zero angles.");
        while (!_button.is_pressed())
        {
            _button.read();
        }

        _gyro.set_zero_angles();

        Serial.println("Zero angles set.");
    }
    void loop(int speed)
    {
        _gyro.read();
        gyroData data = _gyro.data();

        _m.set_velocity(speed, MOVE_ANGLE, ALIGN_ANGLE, data);
        _m.move();
    }

private:
    MovementController _m;
    BNO055 _gyro;
    Button _button;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

MovementController movement_controller(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles,
    new PIDControl(config.controls_config().pid));

BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);

MovementControllerTest test(movement_controller, gyro, button);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(config.speed());
}
