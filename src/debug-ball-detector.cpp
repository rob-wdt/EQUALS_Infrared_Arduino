#include <Arduino.h>
#include "config.h"
#include "control/ball-detector/ball-detector.h"

#define THRESHOLD 1

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
        Serial.println(THRESHOLD);
    }

private:
    BallDetector _b;
};

BallDetector ball_detector(Phototransistor(Pin::PIN_PHOTOTRANSISTOR), THRESHOLD);

BallDetectorTest test(ball_detector);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
