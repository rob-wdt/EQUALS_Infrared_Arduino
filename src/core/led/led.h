#pragma once

#include <Arduino.h>
#include "core/device.h"

class Led : public Device
{
public:
    Led(uint8_t pin);
    void init() override;
    void on();
    void off();

private:
    uint8_t _pin;
};
