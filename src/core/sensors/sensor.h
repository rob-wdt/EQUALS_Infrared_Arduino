#pragma once
#include "core/device.h"

class Sensor : public Device
{
public:
    virtual void read() = 0;
};
