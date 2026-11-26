#include <Arduino.h>
#include "config.h"
#include "utils/control/PID-control/PID-control.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "management/config/robot-config.h"
#include "debug/debug-controls.h"

class PDCControlTest
{
public:
    PDCControlTest(PDCControl pdc, BNO055 gyro, DebugControls controls, Led led) : _pdc(pdc), _gyro(gyro), _controls(controls), _led(led) {}

    void setup()
    {
        _gyro.init();
        _controls.init();
        _led.init();
        Serial.begin(115200);

        _controls.calibrate_and_set_zero(_gyro, _led);
        _controls.wait_start();
    }
    void loop()
    {
        if (!_controls.update(_gyro))
            return;
        _gyro.read();
        gyroData data = _gyro.data();

        double pid_output = _pdc.calculate(data.yaw);

        Serial.println(pid_output);
    }

private:
    PDCControl _pdc;
    BNO055 _gyro;
    DebugControls _controls;
    Led _led;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

PDCControl pdc(config.controls_config().pdc);
BNO055 gyro(config.gyro_address());
DebugControls controls(config.pins().btn1, config.pins().btn2, config.pins().btn3);
Led led(config.pins().led1);

PDCControlTest test(pdc, gyro, controls, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
