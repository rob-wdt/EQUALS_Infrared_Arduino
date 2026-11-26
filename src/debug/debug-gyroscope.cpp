#include <Arduino.h>
#include "config.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "management/config/robot-config.h"
#include "debug/debug-controls.h"

class GyroscopeTest
{
public:
    GyroscopeTest(BNO055 gyro, DebugControls controls, Led led) : _gyro(gyro), _controls(controls), _led(led) {}
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

        Serial.print("Gyro data: ");
        Serial.print("Roll: ");
        Serial.print(data.roll);
        Serial.print(", Pitch: ");
        Serial.print(data.pitch);
        Serial.print(", Yaw: ");
        Serial.println(data.yaw);
    }

private:
    BNO055 _gyro;
    DebugControls _controls;
    Led _led;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

BNO055 gyro(config.gyro_address());
DebugControls controls(config.pins().btn1, config.pins().btn2, config.pins().btn3);
Led led(config.pins().led1);
GyroscopeTest test(gyro, controls, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
