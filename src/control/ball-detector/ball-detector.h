#pragma once

#include "core/sensors/phototransistor/phototransistor.h"

class BallDetector
{
public:
    BallDetector(Phototransistor phototransistor, uint16_t threshold, bool detected_above_threshold = false);
    void init();
    void read();
    bool ball_detected();
    uint16_t phototransistor_data();
    int threshold();

private:
    Phototransistor _phototransistor;
    uint16_t _threshold;
    bool _detected_above_threshold;
};
