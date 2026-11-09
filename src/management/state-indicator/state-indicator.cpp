#include "state-indicator.h"

StateIndicator::StateIndicator(Led led1, Led led2, Led led3) : _l1(led1), _l2(led2), _l3(led3) {}

void StateIndicator::init()
{
    _l1.init();
    _l2.init();
    _l3.init();
}

void StateIndicator::indicate(const State &state, StateContext ctx)
{
    if (state.role() == RoleKind::NONE)
    {
        _l1.off();
        _l2.off();
        _l3.off();
    }
    else if (state.role() == RoleKind::DEFENDER)
    {
        if (state.name() == ctx.states->defender_return.name())
        {
            _l1.off();
            _l2.off();
            _l3.off();
        }
        else if (state.name() == ctx.states->defender_guard_front.name())
        {
            _l1.off();
            _l2.off();
            _l3.on();
        }
        else if (state.name() == ctx.states->defender_guard_left.name())
        {
            _l1.off();
            _l2.on();
            _l3.off();
        }
        else if (state.name() == ctx.states->defender_guard_right.name())
        {
            _l1.off();
            _l2.on();
            _l3.on();
        }
    }
    else if (state.role() == RoleKind::ATTACKER_DRIBBLER)
    {
        if (state.name() == ctx.states->attacker_dribbler_search.name())
        {
            _l1.off();
            _l2.off();
            _l3.off();
        }
        else if (state.name() == ctx.states->attacker_dribbler_approach.name())
        {
            _l1.off();
            _l2.off();
            _l3.on();
        }
        else if (state.name() == ctx.states->attacker_dribbler_capture.name())
        {
            _l1.off();
            _l2.on();
            _l3.off();
        }
        else if (state.name() == ctx.states->attacker_dribbler_drive.name())
        {
            _l1.off();
            _l2.on();
            _l3.on();
        }
        else if (state.name() == ctx.states->attacker_dribbler_kick.name())
        {
            _l1.on();
            _l2.off();
            _l3.off();
        }
    }
    else if (state.role() == RoleKind::ATTACKER_SIMPLE)
    {
        if (state.name() == ctx.states->attacker_simple_search.name())
        {
            _l1.off();
            _l2.off();
            _l3.off();
        }
        else if (state.name() == ctx.states->attacker_simple_approach.name())
        {
            _l1.off();
            _l2.off();
            _l3.on();
        }
        else if (state.name() == ctx.states->attacker_simple_capture.name())
        {
            _l1.off();
            _l2.on();
            _l3.off();
        }
        else if (state.name() == ctx.states->attacker_simple_orbit.name())
        {
            _l1.off();
            _l2.on();
            _l3.on();
        }
        else if (state.name() == ctx.states->attacker_simple_drive.name())
        {
            _l1.on();
            _l2.off();
            _l3.off();
        }
        else if (state.name() == ctx.states->attacker_simple_kick.name())
        {
            _l1.on();
            _l2.off();
            _l3.on();
        }
    }
}
