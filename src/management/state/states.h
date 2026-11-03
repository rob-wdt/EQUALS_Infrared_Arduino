#pragma once

#include "management/state/state.h"
#include "utils/timer/timer.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------ COMMON ------------------------------------------------------ //

class StateCalibrating : public State
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "Calibrating"; }
};

class StateIdle : public State
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "Idle"; }
};

class StateOutRecovery : public State
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "OutRecovery"; }
    void set_resume(State *state);

private:
    State *_resume = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- ATTACKER ------------------------------------------------------ //

class StateAttacker : public State
{
};

class StateAttackerDribbler : public StateAttacker
{
    RoleKind role() const override { return RoleKind::ATTACKER_DRIBBLER; }
};

class StateAttackerSimple : public StateAttacker
{
};

class StateAttackerDribblerSearch : public StateAttackerDribbler
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDribblerSearch"; }
    bool can_switch_role() const override { return true; }
};

class StateAttackerDribblerApproach : public StateAttackerDribbler
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDribblerApproach"; }
    bool can_switch_role() const override { return false; }

private:
    bool _lined_up = false; // behind the ball on the goal -> ball line: drive straight at the ball
};

class StateAttackerDribblerCapture : public StateAttackerDribbler
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDribblerCapture"; }
    bool can_switch_role() const override { return false; }

private:
    Timer _timer;
    bool _holding = false; // the phototransistor sees the ball, _timer counts from that moment
};

class StateAttackerDribblerAlignToGoal : public StateAttackerDribbler
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDribblerAlignToGoal"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerDribblerDrive : public StateAttackerDribbler
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDribblerDrive"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerDribblerKick : public StateAttackerDribbler
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDribblerKick"; }

private:
    Timer _timer;
    bool _fired;
};

class StateAttackerSimpleSearch : public StateAttackerSimple
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSimpleSearch"; }
    bool can_switch_role() const override { return true; }
};

class StateAttackerSimpleApproach : public StateAttackerSimple
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSimpleSearch"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerSimpleOrbit : public StateAttackerSimple
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSimpleOrbit"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerSimpleCapture : public StateAttackerSimple
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSimpleCapture"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerSimpleDrive : public StateAttackerSimple
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSimpleOrbit"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerSimpleKick : public StateAttackerSimple
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSimpleOrbit"; }
    bool can_switch_role() const override { return false; }

private:
    Timer _timer;
    bool _fired;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- DEFENDER ------------------------------------------------------ //

class StateDefender : public State
{
public:
    RoleKind role() const override { return RoleKind::DEFENDER; }
};

class StateDefenderReturn : public StateDefender
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "DefenderReturn"; }
    bool can_switch_role() const override { return true; }
};

// Common part of the guard states: drives to the guard point and stands still there
class StateDefenderGuard : public StateDefender
{
public:
    void enter(StateContext &ctx) override { _started = false; }

protected:
    void _guard(StateContext &ctx, Command &cmd, point target);

private:
    bool _started = false; // false: the next point is taken as is, without smoothing
    bool _holding = false; // reached the point: don't move until it is guard_hold_distance away
    point _target;         // smoothed guard point
};

class StateDefenderGuardFront : public StateDefenderGuard
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "DefenderGuardFront"; }
    bool can_switch_role() const override { return true; }
};

class StateDefenderGuardLeft : public StateDefenderGuard
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "DefenderGuardLeft"; }
    bool can_switch_role() const override { return true; }
};

class StateDefenderGuardRight : public StateDefenderGuard
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "DefenderGuardRight"; }
    bool can_switch_role() const override { return true; }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All states live here, so nothing is allocated at runtime
struct StateSet
{
    StateCalibrating calibrating;
    StateIdle idle;
    StateOutRecovery out_recovery;

    StateAttackerDribblerSearch attacker_dribbler_search;
    StateAttackerDribblerApproach attacker_dribbler_approach;
    StateAttackerDribblerCapture attacker_dribbler_capture;
    StateAttackerDribblerAlignToGoal attacker_dribbler_align_to_goal;
    StateAttackerDribblerDrive attacker_dribbler_drive;
    StateAttackerDribblerKick attacker_dribbler_kick;

    StateAttackerSimpleSearch attacker_simple_search;
    StateAttackerSimpleApproach attacker_simple_approach;
    StateAttackerSimpleCapture attacker_simple_capture;
    StateAttackerSimpleOrbit attacker_simple_orbit;
    StateAttackerSimpleDrive attacker_simple_drive;
    StateAttackerSimpleKick attacker_simple_kick;

    StateDefenderReturn defender_return;
    StateDefenderGuardFront defender_guard_front;
    StateDefenderGuardLeft defender_guard_left;
    StateDefenderGuardRight defender_guard_right;
};
