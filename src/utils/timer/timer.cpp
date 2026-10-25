#include "timer.h"
#include <Arduino.h>

Timer::Timer()
{
    _mark = 0;
    _set_time_ms = 0;
}

void Timer::set_time_ms(long time_ms)
{
    _set_time_ms = time_ms;
    _mark = millis();
}

bool Timer::time_is_up()
{
    return millis() - _mark >= _set_time_ms;
}

void Timer::reset()
{
    _mark = millis();
}
