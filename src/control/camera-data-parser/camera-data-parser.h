#pragma once
#include <Arduino.h>
#include "core/UART/UART.h"
#include "management/goal-mapper/goal-mapper.h"
#include "control/camera-data/camera-data.h"

class CameraDataParser
{
public:
    CameraDataParser(UART uart, GoalMapper goal_mapper);
    void init();
    void parse();
    cameraData data();

private:
    UART _uart;
    GoalMapper _goal;
    cameraData _data = {0, 0, 0, 0, 0, 0};
};
