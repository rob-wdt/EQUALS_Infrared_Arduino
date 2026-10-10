#include <Arduino.h>
#include "config.h"
#include "utils/control/PID-control/PID-control.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "management/config/robot-config.h"

class PDCControlTest
{
public:
    PDCControlTest(PDCControl pdc, BNO055 gyro, Button button, Led led) : _pdc(pdc), _gyro(gyro), _button(button), _led(led) {}

    void setup()
    {
        _gyro.init();
        _button.init();
        _led.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to calibrate the gyroscope.");
        _button.wait_click();

        while (!_gyro.calibrated())
            ;
        _led.on();

        Serial.println("Gyroscope calibrated");
        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to set the zero angles.");
        _button.wait_click();
        _gyro.set_zero_angles();

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to start.");
        _button.wait_click();
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
    Led _led;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

PDCControl pdc(config.controls_config().pdc);
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
Led led(config.pins().led);

PDCControlTest test(pdc, gyro, button, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
