#include <Arduino.h>
#include "config.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"

class GyroscopeTest
{
public:
    GyroscopeTest(BNO055 gyro, Button button) : _gyro(gyro), _button(button) {}
    void setup()
    {
        _gyro.init();
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
    Button _button;
};

BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);
GyroscopeTest test(gyro, button);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
