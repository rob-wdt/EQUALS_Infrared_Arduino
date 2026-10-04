#pragma once

#include "control/ball-motion-watcher/ball-motion-watcher.h"
#include "management/state/states.h"

class StateMachine
{
public:
    StateMachine();
    // ctx comes with the inputs filled in by Robot, cmd is the output
    void update(StateContext &ctx, Command &cmd);
    State *current();

private:
    StateSet _states;
    State *_current;
    BallMotionWatcher _ball_watcher;
    RoleKind _role = RoleKind::NONE;
    unsigned long _last_role_switch = 0;

    void _transition_to(State *next, StateContext &ctx);
    State *_global_transition(StateContext &ctx);
    State *_role_switch(StateContext &ctx);
};
