#include "motor.h"

Motor::Motor(uint8_t pin1, uint8_t pin2, MotorDirection direction) : _pin1(pin1), _pin2(pin2), _direction(direction) {}

void Motor::init()
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
}

void Motor::set_speed(int16_t speed) { _speed = speed; }

void Motor::run()
{
    if (_speed > 0)
    {
        digitalWrite(_pin1, HIGH);
        analogWrite(_pin2, min(255 - _speed, 255));
    }
    else
    {
        digitalWrite(_pin2, HIGH);
        analogWrite(_pin1, min(255 + _speed, 255));
    }
}

void Motor::stop()
{
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, HIGH);
}
