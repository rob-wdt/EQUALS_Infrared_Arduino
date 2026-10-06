#include "goal-mapper.h"

GoalMapper::GoalMapper(GoalColor own_goal): _data{cameraData{0, 0, 0, 0, 0, 0}} { _own_goal_blue = own_goal == GoalColor::BLUE; }

void GoalMapper::map(rawCameraData raw)
{
    _data.ball_angle = raw.ball_angle;
    _data.ball_distance = raw.ball_distance;

    if (_own_goal_blue)
    {
        _data.own_goal_angle = raw.blue_angle;
        _data.own_goal_distance = raw.blue_distance;

        _data.opp_goal_angle = raw.yellow_angle;
        _data.opp_goal_distance = raw.yellow_distance;
    }
    else
    {
        _data.own_goal_angle = raw.yellow_angle;
        _data.own_goal_distance = raw.yellow_distance;

        _data.opp_goal_angle = raw.blue_angle;
        _data.opp_goal_distance = raw.blue_distance;
    }
}

cameraData GoalMapper::data() { return _data; }
