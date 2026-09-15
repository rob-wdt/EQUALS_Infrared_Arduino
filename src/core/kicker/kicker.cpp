#include "kicker.h"

Kicker::Kicker(uint8_t pin, int kicking_time_ms) : _pin(pin), _kicking_time_ms(kicking_time_ms) {}

void Kicker::init()
{
    pinMode(_pin, OUTPUT);
    _timer.set_time_ms(_kicking_time_ms);
}

void Kicker::kick()
{
    digitalWrite(_pin, HIGH);
    _timer.reset();
    _unckick = true;
}

void Kicker::unkick()
{
    if (_unckick && _timer.time_is_up())
    {
        digitalWrite(_pin, LOW);
        _timer.reset();
        _unckick = false;
    }
}
