#include "states.h"

// Camera reports 0 when an object is not seen
static bool seen(int distance) { return distance > 0; }

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
    cmd.lin_speed = ctx.params.speed;
    cmd.align_angle = 0;

    switch (ctx.out)
    {
    case OutType::NO:
        return _resume ? _resume : &ctx.states->idle;
    case OutType::LEFT:
        cmd.move_angle = 90;
        break;
    case OutType::RIGHT:
        cmd.move_angle = -90;
        break;
    case OutType::FRONT_GOAL:
        cmd.move_angle = 180;
        break;
    case OutType::REAR_GOAL:
        cmd.move_angle = 0;
        break;
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- ATTACKER ------------------------------------------------------ //

State *StateAttackerSearch::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_captured)
        return &ctx.states->attacker_capture;
    if (ctx.ball_visible)
        return &ctx.states->attacker_approach;

    // Ball is lost: fall back towards own goal and keep looking
    if (seen(ctx.cam.own_goal_distance))
    {
        cmd.lin_speed = ctx.params.speed / 2;
        cmd.move_angle = ctx.cam.own_goal_angle;
    }
    return nullptr;
}

State *StateAttackerApproach::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_captured)
        return &ctx.states->attacker_capture;
    if (!ctx.ball_visible)
        return &ctx.states->attacker_search;

    if (abs(ctx.cam.ball_angle) > ctx.params.orbit_enter_angle && ctx.cam.ball_distance < ctx.params.orbit_distance)
        return &ctx.states->attacker_orbit;

    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.ball_angle;
    return nullptr;
}

State *StateAttackerOrbit::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_captured)
        return &ctx.states->attacker_capture;
    if (!ctx.ball_visible)
        return &ctx.states->attacker_search;

    if (abs(ctx.cam.ball_angle) < ctx.params.orbit_exit_angle || ctx.cam.ball_distance >= ctx.params.orbit_distance)
        return &ctx.states->attacker_approach;

    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.ball_angle + (ctx.cam.ball_angle > 0 ? 90 : -90);
    return nullptr;
}

void StateAttackerCapture::enter(StateContext &ctx) { _timer.set_time_ms(ctx.params.capture_ms); }

State *StateAttackerCapture::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_captured)
        return &ctx.states->attacker_approach;

    cmd.lin_speed = ctx.params.speed / 2;
    cmd.move_angle = 0;

    if (_timer.time_is_up())
        return &ctx.states->attacker_drive;
    return nullptr;
}

void StateAttackerDrive::enter(StateContext &ctx) { _timer.set_time_ms(ctx.params.drive_max_ms); }

State *StateAttackerDrive::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_captured)
        return &ctx.states->attacker_approach;

    bool goal_seen = seen(ctx.cam.opp_goal_distance);

    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = goal_seen ? ctx.cam.opp_goal_angle : 0;

    if ((goal_seen && ctx.cam.opp_goal_distance <= ctx.params.kick_goal_distance) || _timer.time_is_up())
        return &ctx.states->attacker_kick;
    return nullptr;
}

void StateAttackerKick::enter(StateContext &ctx)
{
    _timer.set_time_ms(ctx.params.kick_cooldown_ms);
    _fired = false;
}

State *StateAttackerKick::update(StateContext &ctx, Command &cmd)
{
    if (!_fired)
    {
        cmd.kick = true;
        _fired = true;
    }

    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = 0;

    if (_timer.time_is_up())
        return &ctx.states->attacker_approach;
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- DEFENDER ------------------------------------------------------ //

State *StateDefenderReturn::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_captured || (ctx.ball_visible && ctx.cam.ball_distance <= ctx.params.clear_distance))
        return &ctx.states->defender_clear;

    if (!seen(ctx.cam.own_goal_distance))
    {
        // Lost own goal: reverse until the camera finds it
        cmd.lin_speed = ctx.params.speed / 2;
        cmd.move_angle = 180;
        return nullptr;
    }

    if (ctx.cam.own_goal_distance <= ctx.params.guard_distance)
        return &ctx.states->defender_guard;

    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.own_goal_angle;
    return nullptr;
}

State *StateDefenderGuard::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_captured || (ctx.ball_visible && ctx.cam.ball_distance <= ctx.params.clear_distance))
        return &ctx.states->defender_clear;

    if (!seen(ctx.cam.own_goal_distance) || ctx.cam.own_goal_distance > ctx.params.guard_distance + ctx.params.guard_tolerance)
        return &ctx.states->defender_return;

    // Slide along the goal line after the ball
    cmd.move_angle = 90;
    cmd.lin_speed = ctx.ball_visible ? ctx.params.defender_coef * ctx.cam.ball_angle : 0;
    return nullptr;
}

void StateDefenderClear::enter(StateContext &ctx) { _timer.set_time_ms(ctx.params.clear_max_ms); }

State *StateDefenderClear::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_captured)
    {
        cmd.kick = true;
        return &ctx.states->defender_return;
    }

    if (!ctx.ball_visible || ctx.cam.ball_distance > ctx.params.clear_distance + ctx.params.clear_tolerance || _timer.time_is_up())
        return &ctx.states->defender_return;

    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.ball_angle;
    return nullptr;
}
