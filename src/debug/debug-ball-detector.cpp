#include <Arduino.h>
#include "config.h"
#include "control/ball-detector/ball-detector.h"
#include "management/config/robot-config.h"

class BallDetectorTest
{
public:
    BallDetectorTest(BallDetector bd) : _b(bd) {}
    void setup()
    {
        _b.init();
        Serial.begin(115200);
    }
    void loop()
    {
        _b.read();
        Serial.print("Ball detected: ");
        Serial.print(_b.ball_detected());
        Serial.print("\tPhtstr data: ");
        Serial.print(_b.phototransistor_data());
        Serial.print("\tThreshold: ");
        Serial.println(_b.threshold());
    }

private:
    BallDetector _b;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

BallDetector ball_detector(Phototransistor(config.pins().phototransistor), config.phototransistor_threshold(), config.phototransistor_detected_above_threshold());

BallDetectorTest test(ball_detector);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
