#pragma once

#include <Arduino.h>
#include "core/sensors/sensor.h"

class Button : public Sensor
{
public:
    Button(uint8_t pin);
    void init() override;
    void read() override;
    void reset();
    bool is_pressed();

    int pin();

private:
    uint8_t _pin;
    bool _is_pressed;
};
