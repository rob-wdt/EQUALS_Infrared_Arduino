#pragma once

#include "control/ball-motion-watcher/ball-motion-watcher.h"
#include "management/state/states.h"

class StateMachine
{
public:
    StateMachine();
    // ctx comes with the inputs filled in by Robot, cmd is the output
    void update(StateContext &ctx, Command &cmd);
    // Puts the machine into the given state right away (its enter() is called)
    void set_state(State *state, StateContext &ctx);
    // Runs only the given state: its command is returned, the transitions it asks for are ignored
    void run_state(State *state, StateContext &ctx, Command &cmd);
    State *current();
    StateSet &states();

private:
    StateSet _states;
    State *_current;
    BallMotionWatcher _ball_watcher;
    RoleKind _role = RoleKind::NONE;
    unsigned long _last_role_switch = 0;

    // Fills the fields of ctx that the machine derives itself (states, ball history)
    void _prepare(StateContext &ctx);
    void _transition_to(State *next, StateContext &ctx);
    State *_global_transition(StateContext &ctx);
    State *_role_switch(StateContext &ctx);
};
