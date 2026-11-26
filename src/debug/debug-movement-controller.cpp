#include <Arduino.h>
#include "config.h"
#include "control/movement-controller/movement-controller.h"
#include "control/heading-controller/heading-controller.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "management/config/robot-config.h"
#include "debug/debug-controls.h"

// FEEL FREE TO CHANGE
#define SPEED 150
#define MOVE_ANGLE 0
#define ALIGN_ANGLE 0

class MovementControllerTest
{
public:
    MovementControllerTest(MovementController mc, HeadingController align, BNO055 gyro, DebugControls controls, Led led)
        : _m(mc), _align(align), _gyro(gyro), _controls(controls), _led(led) {}

    void setup()
    {
        Serial.begin(115200);

        _gyro.init();
        _m.init();
        _controls.init();
        _led.init();

        _controls.calibrate_and_set_zero(_gyro, _led);
        _controls.wait_start();
    }

    void loop(int speed)
    {
        _gyro.read();

        if (!_controls.update(_gyro))
        {
            _m.stop();
            return;
        }

        gyroData data = _gyro.data();
        _m.set_velocity(speed, MOVE_ANGLE - data.yaw, _align.calculate(ALIGN_ANGLE, data));
        _m.move();
    }

private:
    MovementController _m;
    HeadingController _align;
    BNO055 _gyro;
    DebugControls _controls;
    Led _led;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

MovementController movement_controller(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles);
HeadingController align(new PIDControl(config.controls_config().pid));

BNO055 gyro(config.gyro_address());
DebugControls controls(config.pins().btn1, config.pins().btn2, config.pins().btn3);
Led led(config.pins().led1);

MovementControllerTest test(movement_controller, align, gyro, controls, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(SPEED);
}
