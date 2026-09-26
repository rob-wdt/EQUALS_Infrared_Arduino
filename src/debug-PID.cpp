#include <Arduino.h>
#include "config.h"
#include "utils/control/PID-control/PID-control.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"

class PIDControlTest
{
public:
    PIDControlTest(PIDControl pid, BNO055 gyro, Button button) : _pid(pid), _gyro(gyro), _button(button) {}

    void setup()
    {
        _gyro.init();
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
        gyroData data = _gyro.data();

        double pid_output = _pid.calculate(data.yaw);

        Serial.println(pid_output);
    }

private:
    PIDControl _pid;
    BNO055 _gyro;
    Button _button;
};

PIDControl pid(control_gyro_align_pid);
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

PIDControlTest test(pid, gyro, button);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
