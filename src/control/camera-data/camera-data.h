#pragma once

struct rawCameraData
{
    int ball_angle, ball_distance, blue_angle, blue_distance, yellow_angle, yellow_distance;
};

struct cameraData
{
    int own_goal_angle,
        own_goal_distance,
        opp_goal_angle,
        opp_goal_distance,
        ball_angle,
        ball_distance;
};
