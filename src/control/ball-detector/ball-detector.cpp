#include "ball-detector.h"

BallDetector::BallDetector(Phototransistor phototransistor, uint16_t threshold, bool detected_above_threshold)
    : _phototransistor(phototransistor), _threshold(threshold), _detected_above_threshold(detected_above_threshold)
{
}

void BallDetector::init() { _phototransistor.init(); }

void BallDetector::read() { _phototransistor.read(); }

bool BallDetector::ball_detected()
{
    const uint16_t value = _phototransistor.data();
    return _detected_above_threshold ? value >= _threshold : value <= _threshold;
}

uint16_t BallDetector::phototransistor_data() { return _phototransistor.data(); }

int BallDetector::threshold() { return _threshold; }
