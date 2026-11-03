#include <Arduino.h>

#include "core/sensors/distance-sensor/distance-sensor.h"
#include "management/config/robot-config.h"

class DistanceSensorTest
{
public:
    DistanceSensorTest(DistanceSensor ds) : _ds(ds) {}
    void setup()
    {
        _ds.init();
        Serial.begin(115200);
    }
    void loop()
    {
        _ds.read();

        Serial.print("Distance sensor data:\t");
        Serial.println(_ds.data());
    }

private:
    DistanceSensor _ds;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

DistanceSensor ds(config.pins().distane_sensor);
DistanceSensorTest test(ds);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
