#include <Arduino.h>
#include "config.h"
#include "utils/control/PID-control/PID-control.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "management/config/robot-config.h"

class PDCControlTest
{
public:
    PDCControlTest(PDCControl pdc, BNO055 gyro, Button button) : _pdc(pdc), _gyro(gyro), _button(button) {}

    void setup()
    {
        _gyro.init();
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
        gyroData data = _gyro.data();

        double pid_output = _pdc.calculate(data.yaw);

        Serial.println(pid_output);
    }

private:
    PDCControl _pdc;
    BNO055 _gyro;
    Button _button;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

PDCControl pdc(config.controls_config().pdc);
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);

PDCControlTest test(pdc, gyro, button);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
