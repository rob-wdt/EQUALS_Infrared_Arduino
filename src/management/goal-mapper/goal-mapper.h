#pragma once

#include "control/camera-data/camera-data.h"

enum GoalColor
{
    YELLOW,
    BLUE
};

class GoalMapper
{
public:
    GoalMapper(GoalColor own_goal);
    void map(rawCameraData raw);
    cameraData data();

private:
    bool _own_goal_blue = false;
    cameraData _data;
};
