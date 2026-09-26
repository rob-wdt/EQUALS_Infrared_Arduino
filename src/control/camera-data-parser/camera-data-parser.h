#pragma once
#include <Arduino.h>
#include "core/UART/UART.h"

struct cameraData
{
    int own_goal_angle,
        own_goal_distance,
        opp_goal_angle,
        opp_goal_distance,
        ball_angle,
        ball_distance;
};

class CameraDataParser
{
public:
    CameraDataParser(UART uart);
    void init();
    void parse();
    cameraData data();

private:
    UART _uart;
    cameraData _data = {0, 0, 0, 0, 0, 0};
};
