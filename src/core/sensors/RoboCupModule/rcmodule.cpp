#include <Arduino.h>
#include "rcmodule.h"

RCModule::RCModule(int pin) : _pin(pin) {}

void RCModule::init() { pinMode(_pin, INPUT); }

void RCModule::read() { _data = digitalRead(_pin); }

int RCModule::data() { return _data; }
