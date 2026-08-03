#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

struct gyroData
{
    int16_t pitch, roll, yaw; // from -180 to 180
};

class BNO055
{
public:
    BNO055(uint8_t IIC_address);
    void init();
    bool calibrated();
    void read();
    void set_zero_angles();
    gyroData data();

private:
    Adafruit_BNO055 _bno;
    gyroData _data;
    gyroData _zero_angles;

    int16_t _wrap_angle(int16_t angle);
};
