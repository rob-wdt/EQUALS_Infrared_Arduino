#pragma once

#include "core/sensors/phototransistor/phototransistor.h"

class BallDetector
{
public:
    BallDetector(Phototransistor phototransistor, uint16_t threshold);
    void init();
    void read();
    bool ball_detected();

private:
    Phototransistor _phototransistor;
    uint16_t _threshold;
};
