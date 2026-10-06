#pragma once

#include "Servo.h"

class Dribbler
{
public:
    Dribbler(int pin, int min_val, int max_val);
    void init();
    void run();
    void stop();

private:
    int _pin;
    Servo _servo;
    int _max;
    int _min;
};
