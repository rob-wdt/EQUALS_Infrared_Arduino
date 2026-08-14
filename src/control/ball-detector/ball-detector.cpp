#include "ball-detector.h"

BallDetector::BallDetector(Phototransistor phototransistor, uint16_t threshold)
    : _phototransistor(phototransistor), _threshold(threshold)
{
}

void BallDetector::init() { _phototransistor.init(); }

void BallDetector::read() { _phototransistor.read(); }

bool BallDetector::ball_detected() { return _phototransistor.data() < _threshold; }
