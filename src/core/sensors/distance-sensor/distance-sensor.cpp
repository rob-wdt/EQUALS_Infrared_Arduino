#include <Arduino.h>
#include "distance-sensor.h"

DistanceSensor::DistanceSensor(int pin) : _pin(pin) {}

void DistanceSensor::init() { pinMode(_pin, INPUT); }

void DistanceSensor::read() { _data = analogRead(_pin); }

int DistanceSensor::data() { return _data; }
