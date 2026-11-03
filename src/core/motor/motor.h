#pragma once
#include <Arduino.h>
#include "core/device.h"

enum MotorDirection : int8_t
{
    forward = 1,
    backward = -1
};

class Motor : public Device
{
public:
    Motor(uint8_t pin1, uint8_t pin2, MotorDirection direction);
    void init();
    void set_speed(int16_t speed);
    void run();
    void stop();

    MotorDirection direction() const;

private:
    int16_t _speed;
    uint8_t _pin1;
    uint8_t _pin2;
    int8_t _direction;
};
