#pragma once

#include <Arduino.h>

#include "control/camera-data-parser/camera-data-parser.h"
#include "control/robot-coordinates-calculator/robot-coordinates-calculator.h"
#include "control/outs-handler/outs-handler.h"
#include "core/sensors/BNO055/BNO055.h"

struct StateSet;

enum class RoleKind : uint8_t
{
    NONE,
    ATTACKER,
    DEFENDER
};

// Which regulator turns the robot to Command::align_angle
enum class AlignMode : uint8_t
{
    NORMAL,
    BALL_HOLD // the dribbler holds the ball: softer and slower, so the ball isn't lost on the turn
};

// What the active state wants the robot to do this tick
struct Command
{
    float lin_speed = 0;
    int move_angle = 0;  // relative to the robot front
    int align_angle = 0; // heading in the gyro frame (0 = the direction set at calibration), not a camera angle
    AlignMode align_mode = AlignMode::NORMAL;
    bool stop = false;
    bool kick = false;
    bool calibrate = false;
    bool dribble = false;
};

// Initial values, tune on the field

struct AttackerStateParams
{
    int orbit_enter_angle = 30;           // deg, ball is this far from the front -> start orbiting
    int orbit_exit_angle = 15;            // deg, ball is this close to the front -> stop orbiting
    int orbit_distance = 40;              // cm, orbit only when the ball is closer than this
    unsigned long capture_ms = 500;       // the ball must stay captured this long before Drive (dribbler gets hold of it)
    unsigned long drive_max_ms = 2500;    // kick anyway, the ball must not be held for too long
    int kick_goal_distance = 60;          // cm, kick when the opponent goal is closer than this
    unsigned long kick_cooldown_ms = 400; // pause after the kick
    int behind_ball_distance = 20;        // cm, the approach point lies this far behind the ball (on the goal -> ball line)
    int around_ball_distance = 30;        // cm, side shift of the approach point when the robot is in front of the ball
    int line_up_enter_angle = 25;         // deg, robot this close to the line behind the ball -> drive straight at the ball
    int line_up_exit_angle = 40;          // deg, hysteresis: further than this -> back to the approach point
};

struct DefenderStateParams
{
    int guard_distance = 80; // cm from own goal
    // Guard zone around the own goal (cm, relative to the goal center seen by the camera):
    // a front line y = guard_line_distance from -guard_half_width to +guard_half_width
    // and two side lines from its ends back to the goal line, at guard_side_angle to the goal line
    int guard_line_distance = 40;
    int guard_half_width = 35;
    int guard_side_angle = 60;    // deg, 90 = straight back (a rectangle), less = wider at the goal line
    int guard_slow_distance = 15; // cm, closer to the guard point than this -> slow down, so it doesn't overshoot
    // Coordinates are noisy: the guard point jumps by a few cm every frame and the robot would chase it forever.
    // Closer than guard_arrive_distance -> stop, move again only when the point is guard_hold_distance away
    int guard_arrive_distance = 3; // cm
    int guard_hold_distance = 5;   // cm
    float guard_smoothing = 0.3;   // 0..1, how much the guard point follows a new measurement each frame (1 = no smoothing)
    int guard_tolerance = 10;      // cm, hysteresis for leaving the guard position
    int clear_distance = 35;       // cm, ball is this close -> clear it
    int clear_tolerance = 10;      // cm, hysteresis for giving up the clearance
    unsigned long clear_max_ms = 3000;
};

struct RoleSwitchingStateParams
{
    float ball_still_epsilon = 5;          // cm, smaller displacement counts as "not moving"
    unsigned long role_switch_ms = 5000;   // ball not moving (or lost) for this long -> swap roles
    unsigned long role_cooldown_ms = 5000; // no swaps for this long after the previous one
};
struct StateParams
{
    int speed = 100;
    float defender_coef = 1.5;

    AttackerStateParams attacker;
    DefenderStateParams defender;
    RoleSwitchingStateParams role_switching;
};

// Everything a state may look at. Filled by Robot (inputs) and StateMachine (derived fields)
struct StateContext
{
    cameraData cam;
    gyroData gyro;
    point coords;
    OutType out;
    int out_escape_angle; // where to drive to get back into the field
    bool ball_visible;
    bool ball_captured;
    bool start_pressed;
    bool stop_pressed;
    RoleKind start_role;
    bool can_change_role;
    StateParams params;

    // Derived by StateMachine
    point ball; // field coordinates, valid while ball_visible
    bool opp_goal_visible;
    point opp_goal;      // field coordinates, valid while opp_goal_visible
    bool own_goal_known; // the own goal has been seen at least once
    point own_goal;      // field coordinates, the last place it was seen
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
    // Name for printing and debugging
    virtual const char *name() const = 0;
};
