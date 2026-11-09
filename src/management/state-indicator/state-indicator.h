#pragma once

#include <Arduino.h>
#include "core/led/led.h"
#include "management/state/states.h"

// Uses 3 physical leds on the robot to indicate the state
class StateIndicator
{
public:
    StateIndicator(Led led1, Led led2, Led led3);
    void init();
    void indicate(const State &state, StateContext ctx);

private:
    Led _l1;
    Led _l2;
    Led _l3;
};
