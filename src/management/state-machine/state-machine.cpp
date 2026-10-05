#include "state-machine.h"

StateMachine::StateMachine() : _current(&_states.calibrating) {}

State *StateMachine::current() { return _current; }

void StateMachine::update(StateContext &ctx, Command &cmd)
{
    ctx.states = &_states;

    // Ball position in field coordinates, otherwise a moving robot makes a still ball look like it moves
    float angle = radians(ctx.gyro.yaw + ctx.cam.ball_angle);
    coordinates ball = {ctx.coords.x + ctx.cam.ball_distance * cos(angle),
                        ctx.coords.y + ctx.cam.ball_distance * sin(angle)};
    _ball_watcher.update(ctx.ball_visible, ball, ctx.params.ball_still_epsilon);
    ctx.ball_still_ms = _ball_watcher.still_ms();
    ctx.ball_lost_ms = _ball_watcher.lost_ms();

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
    if (!_current->can_switch_role() || ctx.ball_captured)
        return nullptr;
    if (millis() - _last_role_switch < ctx.params.role_cooldown_ms)
        return nullptr;

    bool still = ctx.ball_still_ms >= ctx.params.role_switch_ms;
    bool lost = ctx.ball_lost_ms >= ctx.params.role_switch_ms;

    switch (_current->role())
    {
    case RoleKind::DEFENDER:
        if (still)
            return &_states.attacker_search;
        break;
    case RoleKind::ATTACKER:
        // Attacker can't make progress: the ball neither moves nor is reachable
        if (still || lost)
            return &_states.defender_return;
        break;
    default:
        break;
    }
    return nullptr;
}
