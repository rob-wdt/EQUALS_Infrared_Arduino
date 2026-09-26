#include "core/motor/motor.h"
#include "config.h"

class MotorTest
{
public:
    MotorTest(int pin1, int pin2, MotorDirection direction) : _motor(pin1, pin2, direction) {}
    void setup()
    {
        _motor.init();
    }
    void loop()
    {
        _motor.set_speed(SPEED);
        _motor.run();
    }

private:
    Motor _motor;
};

MotorTest test(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
