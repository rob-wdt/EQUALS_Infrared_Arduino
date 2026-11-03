#include "state-machine.h"

StateMachine::StateMachine() : _current(&_states.calibrating) {}

State *StateMachine::current() { return _current; }

StateSet &StateMachine::states() { return _states; }

void StateMachine::_prepare(StateContext &ctx)
{
    ctx.states = &_states;

    // Ball and goal in field coordinates: the vectors from the robot to them, moved to the robot position.
    // A still ball has to stay still while the robot moves, otherwise the watcher thinks it moves
    _ball_coords.calculate(ctx.gyro, ctx.cam, ctx.coords);
    ctx.ball = _ball_coords.coordinates();
    ctx.opp_goal_visible = ctx.cam.opp_goal_distance > 0;
    ctx.opp_goal = MathVector::from_angle(ctx.coords, ctx.gyro.yaw + ctx.cam.opp_goal_angle, ctx.cam.opp_goal_distance).end();

    // The defender works relative to its own goal. When it doesn't see the goal for a moment, the last place is used
    if (ctx.cam.own_goal_distance > 0)
    {
        _own_goal = MathVector::from_angle(ctx.coords, ctx.gyro.yaw + ctx.cam.own_goal_angle, ctx.cam.own_goal_distance).end();
        _own_goal_known = true;
    }
    ctx.own_goal_known = _own_goal_known;
    ctx.own_goal = _own_goal;

    _ball_watcher.update(ctx.ball_visible, ctx.ball, ctx.params.role_switching.ball_still_epsilon);
    ctx.ball_still_ms = _ball_watcher.still_ms();
    ctx.ball_lost_ms = _ball_watcher.lost_ms();
}

void StateMachine::update(StateContext &ctx, Command &cmd)
{
    _prepare(ctx);
    cmd = Command();

    // Priorities: global events > role swap > the state's own transitions
    State *next = _global_transition(ctx);
    if (!next)
        next = _role_switch(ctx);

    if (next)
        _transition_to(next, ctx);

    next = _current->update(ctx, cmd);
    if (next)
        _transition_to(next, ctx);
}

void StateMachine::set_state(State *state, StateContext &ctx)
{
    _prepare(ctx);
    _transition_to(state, ctx);
}

void StateMachine::run_state(State *state, StateContext &ctx, Command &cmd)
{
    _prepare(ctx);
    // Entering it once, so its timers start like in a normal game
    _transition_to(state, ctx);
    cmd = Command();
    state->update(ctx, cmd);
}

void StateMachine::_transition_to(State *next, StateContext &ctx)
{
    if (next == _current)
        return;

    // Starting to play or swapping roles: forget the old ball history and start the cooldown
    RoleKind next_role = next->role();
    if (next_role != RoleKind::NONE && next_role != _role)
    {
        _role = next_role;
        _last_role_switch = millis();
        _ball_watcher.reset();
        ctx.ball_still_ms = 0;
        ctx.ball_lost_ms = 0;
    }

    _current = next;
    _current->enter(ctx);
}

State *StateMachine::_global_transition(StateContext &ctx)
{
    bool in_service = _current == &_states.calibrating || _current == &_states.idle;
    if (in_service)
        return nullptr;

    if (ctx.stop_pressed)
    {
        _role = RoleKind::NONE;
        return &_states.idle;
    }

    // Out of bounds is handled the same way for every role, afterwards we resume where we were
    if (ctx.out != OutType::NO && _current != &_states.out_recovery)
    {
        _states.out_recovery.set_resume(_current);
        return &_states.out_recovery;
    }
    return nullptr;
}

State *StateMachine::_role_switch(StateContext &ctx)
{
    if (!ctx.can_change_role || !_current->can_switch_role() || ctx.ball_captured)
        return nullptr;
    if (millis() - _last_role_switch < ctx.params.role_switching.role_cooldown_ms)
        return nullptr;

    bool still = ctx.ball_still_ms >= ctx.params.role_switching.role_switch_ms;
    bool lost = ctx.ball_lost_ms >= ctx.params.role_switching.role_switch_ms;

    switch (_current->role())
    {
    case RoleKind::DEFENDER:
        if (still)
        {
            if (ctx.start_role == RoleKind::ATTACKER_DRIBBLER)
                return &_states.attacker_dribbler_search;
                
            if (ctx.start_role == RoleKind::ATTACKER_SIMPLE)
                return &_states.attacker_simple_search;
        }
        break;
    case RoleKind::ATTACKER_DRIBBLER:
        if (still || lost)
            return &_states.defender_return;
        break;
    case RoleKind::ATTACKER_SIMPLE:
        if (still || lost)
            return &_states.defender_return;
        break;
    default:
        break;
    }
    return nullptr;
}
