#include <Arduino.h>
#include "rcmodule.h"

RCModule::RCModule(int pin) : _pin(pin), _data(0) {}

void RCModule::init() { pinMode(_pin, INPUT); }

void RCModule::read() { _data = digitalRead(_pin); }

bool RCModule::start() { return _data == 1; }
