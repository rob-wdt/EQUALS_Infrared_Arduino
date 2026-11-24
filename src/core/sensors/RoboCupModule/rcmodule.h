#pragma once
#include "core/sensors/sensor.h"

class RCModule : public Sensor
{
public:
    RCModule(int pin);
    void init() override;
    void read() override;
    int data();

private:
    int _pin;
    int _data;
};
