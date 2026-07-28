#pragma once
#include "core/sensors/sensor.h"
#include <Arduino.h>

class Phototransistor : public Sensor
{
public:
    Phototransistor(uint8_t pin);
    void init() override;
    void read() override;
    uint16_t data();

private:
    uint8_t _pin;
    uint16_t _data;
};
