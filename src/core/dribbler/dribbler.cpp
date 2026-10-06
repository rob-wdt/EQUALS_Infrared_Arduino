#include <Arduino.h>
#include "dribbler.h"

Dribbler::Dribbler(int pin, int min_val, int max_val) : _pin(pin), _servo(), _max(max_val), _min(min_val) {}

void Dribbler::init() // Blocking function! Usees delay
{
    _servo.attach(_pin);
    delay(1000);

    run();
    delay(1000);

    stop();
    delay(1000);
}

void Dribbler::run() { _servo.writeMicroseconds(_max); }

void Dribbler::stop() { _servo.writeMicroseconds(_min); }
