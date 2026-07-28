#include "phototransistor.h"

Phototransistor::Phototransistor(uint8_t pin) : _pin(pin) {}

void Phototransistor::init() { pinMode(_pin, INPUT); }

void Phototransistor::read() { _data = analogRead(_pin); }

uint16_t Phototransistor::data() { return _data; }
