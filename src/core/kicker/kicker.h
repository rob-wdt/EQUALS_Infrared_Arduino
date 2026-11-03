#pragma once

#include <Arduino.h>
#include "utils/timer/timer.h"
#include "core/device.h"

class Kicker : public Device
{
public:
    Kicker(uint8_t pin, int kicking_time_ms);
    void init();
    void kick();
    void unkick();

private:
    uint8_t _pin;
    int _kicking_time_ms;
    Timer _timer;
    bool _unckick = false;
};
