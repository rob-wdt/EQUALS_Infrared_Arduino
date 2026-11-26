#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "core/sensors/sensor.h"

struct gyroData
{
    int16_t pitch, roll, yaw; // from -180 to 180
};

struct bnoStatus
{
    uint8_t mode, system, self_test, error;
};

class BNO055 : public Sensor
{
public:
    BNO055(uint8_t IIC_address);
    void init() override;
    bool calibrated();
    void read() override;
    void set_zero_angles();
    gyroData data();
    bnoStatus status();

private:
    Adafruit_BNO055 _bno;
    gyroData _data;
    gyroData _zero_angles;

    int16_t _wrap_angle(int16_t angle);
};
