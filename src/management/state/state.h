#pragma once

#include <Arduino.h>

#include "control/camera-data-parser/camera-data-parser.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "control/outs-handler/outs-handler.h"
#include "core/sensors/BNO055/BNO055.h"

struct StateSet;

enum class RoleKind : uint8_t
{
    NONE,
    ATTACKER,
    DEFENDER
};

// What the active state wants the robot to do this tick
struct Command
{
    int lin_speed = 0;
    int move_angle = 0;
    int align_angle = 0;
    bool stop = false;
    bool kick = false;
    bool calibrate = false;
};

// Initial values, tune on the field
struct StateParams
{
    int speed = 100;
    float defender_coef = 0.5;

    // Attacker
    int orbit_enter_angle = 30;           // deg, ball is this far from the front -> start orbiting
    int orbit_exit_angle = 15;            // deg, ball is this close to the front -> stop orbiting
    int orbit_distance = 40;              // cm, orbit only when the ball is closer than this
    unsigned long capture_ms = 150;       // hold after the phototransistor triggers
    unsigned long drive_max_ms = 2500;    // kick anyway, the ball must not be held for too long
    int kick_goal_distance = 60;          // cm, kick when the opponent goal is closer than this
    unsigned long kick_cooldown_ms = 400; // pause after the kick

    // Defender
    int guard_distance = 30;  // cm from own goal
    int guard_tolerance = 10; // cm, hysteresis for leaving the guard position
    int clear_distance = 35;  // cm, ball is this close -> clear it
    int clear_tolerance = 10; // cm, hysteresis for giving up the clearance
    unsigned long clear_max_ms = 3000;

    // Role switching
    float ball_still_epsilon = 5;          // cm, smaller displacement counts as "not moving"
    unsigned long role_switch_ms = 5000;   // ball not moving (or lost) for this long -> swap roles
    unsigned long role_cooldown_ms = 5000; // no swaps for this long after the previous one
};

// Everything a state may look at. Filled by Robot (inputs) and StateMachine (derived fields)
struct StateContext
{
    cameraData cam;
    gyroData gyro;
    coordinates coords;
    OutType out;
    int out_escape_angle; // where to drive to get back into the field
    bool ball_visible;
    bool ball_captured;
    bool start_pressed;
    bool stop_pressed;
    RoleKind start_role;
    StateParams params;

    // Derived by StateMachine
    unsigned long ball_still_ms;
    unsigned long ball_lost_ms;
    StateSet *states;
};

class State
{
public:
    virtual void enter(StateContext &ctx) {}
    // Fills cmd and returns the next state, nullptr means "stay"
    virtual State *update(StateContext &ctx, Command &cmd) = 0;
    virtual RoleKind role() const { return RoleKind::NONE; }
    // Role swap is allowed only from "calm" states, not while capturing or kicking
    virtual bool can_switch_role() const { return false; }
};
