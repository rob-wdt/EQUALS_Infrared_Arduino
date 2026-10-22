#pragma once

#include "management/state/state.h"
#include "control/timer/timer.h"

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

// class StateAttackerSearch : public StateAttacker
// {
// public:
//     State *update(StateContext &ctx, Command &cmd) override;
//     const char *name() const override { return "AttackerSearch"; }
//     bool can_switch_role() const override { return true; }
// };

// class StateAttackerApproach : public StateAttacker
// {
// public:
//     State *update(StateContext &ctx, Command &cmd) override;
//     const char *name() const override { return "AttackerApproach"; }
//     bool can_switch_role() const override { return true; }
// };

// class StateAttackerOrbit : public StateAttacker
// {
// public:
//     State *update(StateContext &ctx, Command &cmd) override;
//     const char *name() const override { return "AttackerOrbit"; }
//     bool can_switch_role() const override { return true; }
// };

// class StateAttackerCapture : public StateAttacker
// {
// public:
//     void enter(StateContext &ctx) override;
//     State *update(StateContext &ctx, Command &cmd) override;
//     const char *name() const override { return "AttackerCapture"; }
//     bool can_switch_role() const override { return false; }

// private:
//     Timer _timer;
// };

// class StateAttackerDrive : public StateAttacker
// {
// public:
//     void enter(StateContext &ctx) override;
//     State *update(StateContext &ctx, Command &cmd) override;
//     const char *name() const override { return "AttackerDrive"; }
//     bool can_switch_role() const override { return false; }

// private:
//     Timer _timer;
// };

// class StateAttackerKick : public StateAttacker
// {
// public:
//     void enter(StateContext &ctx) override;
//     State *update(StateContext &ctx, Command &cmd) override;
//     const char *name() const override { return "AttackerKick"; }

// private:
//     Timer _timer;
//     bool _fired;
// };

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
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "AttackerApproach"; }
    bool can_switch_role() const override { return false; }
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

class StateDefenderGuard : public StateDefender
{
public:
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "DefenderGuard"; }
    bool can_switch_role() const override { return true; }
};

class StateDefenderClear : public StateDefender
{
public:
    void enter(StateContext &ctx) override;
    State *update(StateContext &ctx, Command &cmd) override;
    const char *name() const override { return "DefenderClear"; }

private:
    Timer _timer;
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
    StateAttackerDrive attacker_drive;
    StateAttackerKick attacker_kick;

    StateDefenderReturn defender_return;
    StateDefenderGuard defender_guard;
    StateDefenderClear defender_clear;
};
