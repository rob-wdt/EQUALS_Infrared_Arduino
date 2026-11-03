#include "outs-handler-distance-sensor.h"

void OutsHandlerDistanceSensor::handle(int rear_range, int threshold) { _in_out = rear_range <= threshold; }

bool OutsHandlerDistanceSensor::in_out() { return _in_out; }
