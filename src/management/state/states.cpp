#include "states.h"

// Camera reports 0 when an object is not seen
static bool seen(int distance) { return distance > 0; }

// Heading at which an object seen at relative_angle (camera angle, 0 = in front of the robot) is straight ahead.
// Command::align_angle is a heading in the gyro frame, a camera angle has to be turned into it
static int heading_to(StateContext &ctx, int relative_angle) { return ctx.gyro.yaw + relative_angle; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------ COMMON ------------------------------------------------------- //

State *StateCalibrating::update(StateContext &ctx, Command &cmd)
{
    cmd.stop = true;
    cmd.calibrate = true; // blocking, Robot returns here when the gyroscope is ready
    return &ctx.states->idle;
}

State *StateIdle::update(StateContext &ctx, Command &cmd)
{
    cmd.stop = true;

    if (ctx.start_pressed)
    {
        if (ctx.start_role == RoleKind::DEFENDER)
            return &ctx.states->defender_return;
        return &ctx.states->attacker_search;
    }
    return nullptr;
}

void StateOutRecovery::set_resume(State *state) { _resume = state; }

State *StateOutRecovery::update(StateContext &ctx, Command &cmd)
{
    // OutsHandler keeps the out until the robot is back inside by the hysteresis
    if (ctx.out == OutType::NO)
        return _resume ? _resume : &ctx.states->idle;

    cmd.lin_speed = ctx.params.speed;
    // The escape angle is in field coordinates, move_angle is relative to the robot
    cmd.move_angle = ctx.out_escape_angle - ctx.gyro.yaw;
    cmd.align_angle = 0;
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- ATTACKER ------------------------------------------------------ //

State *StateAttackerSearch::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_visible)
        return &ctx.states->attacker_approach;

    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = 180;

    return nullptr;
}

State *StateAttackerApproach::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_visible)
        return &ctx.states->attacker_search;

    if (ctx.cam.ball_distance <= 20)
        return &ctx.states->attacker_capture;

    cmd.align_angle = heading_to(ctx, ctx.cam.ball_angle);
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = 0;

    return nullptr;
}

void StateAttackerCapture::enter(StateContext &ctx) { _holding = false; }

State *StateAttackerCapture::update(StateContext &ctx, Command &cmd)
{
    // Drive only when the ball has been captured for capture_ms without a break,
    // so the dribbler has time to get hold of it. Losing the ball starts the count again
    if (!ctx.ball_captured)
        _holding = false;
    else if (!_holding)
    {
        _holding = true;
        _timer.set_time_ms(ctx.params.capture_ms);
    }
    else if (_timer.time_is_up())
        return &ctx.states->attacker_drive;

    if (ctx.cam.ball_distance > 20)
        return &ctx.states->attacker_approach;

    cmd.align_angle = heading_to(ctx, ctx.cam.ball_angle);
    cmd.dribble = true;
    cmd.kick = false;
    cmd.lin_speed = 0.7 * ctx.params.speed;
    cmd.move_angle = 0;

    return nullptr;
}

State *StateAttackerDrive::update(StateContext &ctx, Command &cmd)
{
    if (seen(ctx.cam.opp_goal_distance) && ctx.cam.opp_goal_distance <= ctx.params.kick_goal_distance)
        return &ctx.states->attacker_kick;

    if (!ctx.ball_captured)
        return &ctx.states->attacker_approach;

    if (seen(ctx.cam.opp_goal_distance))
    {
        cmd.align_angle = heading_to(ctx, ctx.cam.opp_goal_angle);
        cmd.move_angle = ctx.cam.opp_goal_angle;
    }
    else
    {
        cmd.align_angle = 0;
        cmd.move_angle = 0;
    }

    cmd.dribble = true;
    cmd.kick = false;
    cmd.lin_speed = 1.2 * ctx.params.speed;

    return nullptr;
}

void StateAttackerKick::enter(StateContext &ctx)
{
    _timer.set_time_ms(ctx.params.kick_cooldown_ms);
    _fired = false;
}

State *StateAttackerKick::update(StateContext &ctx, Command &cmd)
{
    if (_timer.time_is_up())
        return &ctx.states->attacker_search;

    if (seen(ctx.cam.opp_goal_distance))
    {
        cmd.align_angle = heading_to(ctx, ctx.cam.opp_goal_angle);
        cmd.move_angle = ctx.cam.opp_goal_angle;
    }
    else
    {
        cmd.align_angle = 0;
        cmd.move_angle = 0;
    }

    if (!_fired)
    {
        cmd.kick = true;
        _fired = true;
    }

    cmd.dribble = true;
    cmd.lin_speed = ctx.params.speed;
    cmd.stop = false;

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- DEFENDER ------------------------------------------------------ //

// State *StateDefenderReturn::update(StateContext &ctx, Command &cmd)
// {
//     if (ctx.ball_captured || (ctx.ball_visible && ctx.cam.ball_distance <= ctx.params.clear_distance))
//         return &ctx.states->defender_clear;

//     if (!seen(ctx.cam.own_goal_distance))
//     {
//         // Lost own goal: reverse until the camera finds it
//         cmd.lin_speed = ctx.params.speed / 2;
//         cmd.move_angle = 180;
//         return nullptr;
//     }

//     if (ctx.cam.own_goal_distance <= ctx.params.guard_distance)
//         return &ctx.states->defender_guard;

//     cmd.lin_speed = ctx.params.speed;
//     cmd.move_angle = ctx.cam.own_goal_angle;
//     return nullptr;
// }

// State *StateDefenderGuard::update(StateContext &ctx, Command &cmd)
// {
//     if (ctx.ball_captured || (ctx.ball_visible && ctx.cam.ball_distance <= ctx.params.clear_distance))
//         return &ctx.states->defender_clear;

//     if (!seen(ctx.cam.own_goal_distance) || ctx.cam.own_goal_distance > ctx.params.guard_distance + ctx.params.guard_tolerance)
//         return &ctx.states->defender_return;

//     // Slide along the goal line after the ball
//     cmd.move_angle = 90;
//     cmd.lin_speed = ctx.ball_visible ? ctx.params.defender_coef * ctx.cam.ball_angle : 0;
//     return nullptr;
// }

// void StateDefenderClear::enter(StateContext &ctx) { _timer.set_time_ms(ctx.params.clear_max_ms); }

// State *StateDefenderClear::update(StateContext &ctx, Command &cmd)
// {
//     if (ctx.ball_captured)
//     {
//         cmd.kick = true;
//         return &ctx.states->defender_return;
//     }

//     if (!ctx.ball_visible || ctx.cam.ball_distance > ctx.params.clear_distance + ctx.params.clear_tolerance || _timer.time_is_up())
//         return &ctx.states->defender_return;

//     cmd.lin_speed = ctx.params.speed;
//     cmd.move_angle = ctx.cam.ball_angle;
//     return nullptr;
// }

State *StateDefenderReturn::update(StateContext &ctx, Command &cmd)
{
    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.own_goal_angle;
    return nullptr;
}

State *StateDefenderGuard::update(StateContext &ctx, Command &cmd)
{
    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.defender_coef * (ctx.cam.ball_angle % 180);
    cmd.move_angle = 90;
    return nullptr;
}

void StateDefenderClear::enter(StateContext &ctx) { _timer.set_time_ms(ctx.params.clear_max_ms); }

State *StateDefenderClear::update(StateContext &ctx, Command &cmd)
{
    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.ball_angle;
    return nullptr;
}
