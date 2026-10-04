#include "core/sensors/phototransistor/phototransistor.h"
#include "config.h"
#include "management/config/robot-config.h"

class PhototransistorTest
{
public:
    PhototransistorTest(int pin) : _phototransistor(pin) {}
    void setup()
    {
        _phototransistor.init();
        Serial.begin(115200);
    }
    void loop()
    {
        _phototransistor.read();
        Serial.print("Phototr. data: ");
        Serial.println(_phototransistor.data());
    }

private:
    Phototransistor _phototransistor;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

PhototransistorTest test(config.pins().phototransistor);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
