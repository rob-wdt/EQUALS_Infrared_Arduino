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
public:
    RoleKind role() const override { return RoleKind::ATTACKER; }
};

class StateAttackerSearch : public StateAttacker
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerSearch"; }
    bool can_switch_role() const override { return true; }
};

class StateAttackerApproach : public StateAttacker
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerApproach"; }
    bool can_switch_role() const override { return false; }

private:
    bool _lined_up = false; // behind the ball on the goal -> ball line: drive straight at the ball
};

class StateAttackerCapture : public StateAttacker
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerCapture"; }
    bool can_switch_role() const override { return false; }

private:
    Timer _timer;
    bool _holding = false; // the phototransistor sees the ball, _timer counts from that moment
};

class StateAttackerAlignToGoal : public StateAttacker
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerAlignToGoal"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerDrive : public StateAttacker
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerDrive"; }
    bool can_switch_role() const override { return false; }
};

class StateAttackerKick : public StateAttacker
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerKick"; }

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

    StateAttackerSearch attacker_search;
    StateAttackerApproach attacker_approach;
    StateAttackerCapture attacker_capture;
    StateAttackerAlignToGoal attacker_align_to_goal;
    StateAttackerDrive attacker_drive;
    StateAttackerKick attacker_kick;

    StateDefenderReturn defender_return;
    StateDefenderGuardFront defender_guard_front;
    StateDefenderGuardLeft defender_guard_left;
    StateDefenderGuardRight defender_guard_right;
};
