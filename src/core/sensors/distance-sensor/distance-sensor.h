#pragma once

#include "core/sensors/sensor.h"

class DistanceSensor : public Sensor
{
public:
    DistanceSensor(int pin);
    void init();
    virtual void read() override;
    int data();

private:
    int _pin;
    int _data;
};
