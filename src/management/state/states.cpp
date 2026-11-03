#include "states.h"
#include "control/point-navigator/point-navigator.h"
#include "utils/math-vector/math-vector.h"
#include "utils/function-intersector/function-intersector.h"
#include "utils/function-intersector/function-intersector.h"

// Heading at which an object seen at relative_angle (camera angle, 0 = in front of the robot) is straight ahead.
// Command::align_angle is a heading in the gyro frame, a camera angle has to be turned into it
static int heading_to(StateContext &ctx, int relative_angle) { return ctx.gyro.yaw + relative_angle; }

// Drives to a point in field coordinates: the navigator gives a field heading, move_angle is relative to the robot
static void move_to(StateContext &ctx, Command &cmd, point target)
{
    PointNavigator navigator;
    navigator.compute(ctx.coords, target);
    cmd.move_angle = navigator.data().move_angle - ctx.gyro.yaw;
}

// Angle between the ball -> robot vector and the line behind the ball (pointing away from the goal), 0..180 deg.
// 0: the robot is right behind the ball and faces the goal through it, 180: the robot is between the ball and the goal
static float angle_behind_ball(StateContext &ctx)
{
    MathVector from_goal = MathVector(ctx.opp_goal, ctx.ball).unit();
    MathVector to_robot = MathVector(ctx.ball, ctx.coords).unit();
    return degrees(acos(constrain(dot(from_goal, to_robot), -1.0f, 1.0f)));
}

// Where to drive to get behind the ball. In front of the ball the point is shifted to the robot's side,
// so the robot goes around the ball and doesn't push it towards its own goal
static point approach_point(StateContext &ctx)
{
    MathVector from_goal = MathVector(ctx.opp_goal, ctx.ball).unit();
    MathVector behind = parallel_transition(from_goal, ctx.ball) * ctx.params.attacker.behind_ball_distance;

    if (angle_behind_ball(ctx) < 90)
        return behind.end();

    // Perpendicular to the goal -> ball line, on the side where the robot is
    MathVector side(ctx.ball, from_goal.y_projection(), -from_goal.x_projection());
    if (dot(side, MathVector(ctx.ball, ctx.coords)) < 0)
        side = side * -1;
    return (behind + side * ctx.params.attacker.around_ball_distance).end();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------ COMMON ------------------------------------------------------- //

State *StateCalibrating::update(StateContext &ctx, Command &cmd)
{
    cmd.stop = true;
    cmd.calibrate = true; // blocking, Robot returns here when the gyroscope is ready
    return &ctx.states->idle;
}

State *StateIdle::update(StateContext &ctx, Command &cmd)
{
    cmd.stop = true;

    if (ctx.start_pressed)
    {
        if (ctx.start_role == RoleKind::DEFENDER)
            return &ctx.states->defender_return;

        if (ctx.start_role == RoleKind::ATTACKER_DRIBBLER)
            return &ctx.states->attacker_dribbler_search;

        if (ctx.start_role == RoleKind::ATTACKER_SIMPLE)
            return &ctx.states->attacker_simple_search;
    }
    return nullptr;
}

void StateOutRecovery::set_resume(State *state) { _resume = state; }

State *StateOutRecovery::update(StateContext &ctx, Command &cmd)
{
    // OutsHandler keeps the out until the robot is back inside by the hysteresis
    if (ctx.out == OutType::NO)
        return _resume ? _resume : &ctx.states->idle;

    cmd.lin_speed = ctx.params.speed;
    // The escape angle is in field coordinates, move_angle is relative to the robot
    cmd.move_angle = ctx.out_escape_angle - ctx.gyro.yaw;
    cmd.align_angle = 0;
    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------ ATTACKER DRIBBLER -------------------------------------------------- //

State *StateAttackerDribblerSearch::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_visible)
        return &ctx.states->attacker_dribbler_approach;

    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = 180;

    return nullptr;
}

void StateAttackerDribblerApproach::enter(StateContext &ctx) { _lined_up = false; }

State *StateAttackerDribblerApproach::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_visible)
        return &ctx.states->attacker_dribbler_search;

    point target = ctx.ball;

    // Without the goal there is no "behind the ball": drive straight at it
    if (!ctx.opp_goal_visible)
        _lined_up = true;
    else
    {
        float angle = angle_behind_ball(ctx);
        if (angle < ctx.params.attacker.line_up_enter_angle)
            _lined_up = true;
        else if (angle > ctx.params.attacker.line_up_exit_angle)
            _lined_up = false;

        if (!_lined_up)
            target = approach_point(ctx);
    }

    if (_lined_up && ctx.cam.ball_distance <= 20)
        return &ctx.states->attacker_dribbler_capture;

    move_to(ctx, cmd, target);

    cmd.align_angle = heading_to(ctx, ctx.cam.ball_angle);
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;

    return nullptr;
}

void StateAttackerDribblerCapture::enter(StateContext &ctx) { _holding = false; }

State *StateAttackerDribblerCapture::update(StateContext &ctx, Command &cmd)
{
    // Drive only when the ball has been captured for capture_ms without a break,
    // so the dribbler has time to get hold of it. Losing the ball starts the count again
    if (!ctx.ball_captured)
        _holding = false;
    else if (!_holding)
    {
        _holding = true;
        _timer.set_time_ms(ctx.params.attacker.capture_ms);
    }
    else if (_timer.time_is_up())
        return &ctx.states->attacker_dribbler_align_to_goal;

    if (ctx.cam.ball_distance > 20)
        return &ctx.states->attacker_dribbler_approach;

    cmd.align_angle = heading_to(ctx, ctx.cam.ball_angle);
    cmd.dribble = true;
    cmd.kick = false;
    cmd.lin_speed = 0.7 * ctx.params.speed;
    cmd.move_angle = 0;

    return nullptr;
}

State *StateAttackerDribblerAlignToGoal::update(StateContext &ctx, Command &cmd)
{
    if (abs(ctx.cam.opp_goal_angle) <= 30)
        return &ctx.states->attacker_dribbler_drive;

    if (ctx.opp_goal_visible)
    {
        cmd.align_angle = MathVector(ctx.coords, ctx.opp_goal).angle();
        move_to(ctx, cmd, ctx.opp_goal);
    }
    else
    {
        cmd.align_angle = 0;
        cmd.move_angle = 0;
    }

    cmd.align_mode = AlignMode::BALL_HOLD;
    cmd.dribble = true;
    cmd.kick = false;
    cmd.lin_speed = 0;

    return nullptr;
}

State *StateAttackerDribblerDrive::update(StateContext &ctx, Command &cmd)
{
    if (ctx.opp_goal_visible && ctx.cam.opp_goal_distance <= ctx.params.attacker.kick_goal_distance)
        return &ctx.states->attacker_dribbler_kick;

    if (!ctx.ball_captured)
        return &ctx.states->attacker_dribbler_approach;

    if (ctx.opp_goal_visible)
    {
        cmd.align_angle = MathVector(ctx.coords, ctx.opp_goal).angle();
        move_to(ctx, cmd, ctx.opp_goal);
    }
    else
    {
        cmd.align_angle = 0;
        cmd.move_angle = 0;
    }

    cmd.align_mode = AlignMode::BALL_HOLD;
    cmd.dribble = true;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;

    return nullptr;
}

void StateAttackerDribblerKick::enter(StateContext &ctx)
{
    _timer.set_time_ms(ctx.params.attacker.kick_cooldown_ms);
    _fired = false;
}

State *StateAttackerDribblerKick::update(StateContext &ctx, Command &cmd)
{
    if (_timer.time_is_up())
        return &ctx.states->attacker_dribbler_search;

    if (ctx.opp_goal_visible)
    {
        cmd.align_angle = MathVector(ctx.coords, ctx.opp_goal).angle();
        move_to(ctx, cmd, ctx.opp_goal);
    }
    else
    {
        cmd.align_angle = 0;
        cmd.move_angle = 0;
    }

    if (!_fired)
    {
        cmd.kick = true;
        _fired = true;
    }

    cmd.dribble = true;
    cmd.lin_speed = ctx.params.speed;
    cmd.stop = false;

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------- ATTACKER SIMPLE --------------------------------------------------- //

State *StateAttackerSimpleSearch::update(StateContext &ctx, Command &cmd)
{
    if (ctx.ball_visible)
        return &ctx.states->attacker_simple_approach;

    cmd.align_angle = 0;
    cmd.align_mode = AlignMode::NORMAL;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.own_goal_angle;

    return nullptr;
}

State *StateAttackerSimpleApproach::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_visible)
        return &ctx.states->attacker_simple_search;

    if (ctx.cam.ball_distance <= ctx.params.attacker.orbit_distance && fabs(ctx.cam.ball_angle) > ctx.params.attacker.orbit_enter_angle)
        return &ctx.states->attacker_simple_orbit;

    cmd.align_angle = 0;
    cmd.align_mode = AlignMode::NORMAL;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.ball_angle;

    return nullptr;
}

State *StateAttackerSimpleOrbit::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_visible)
        return &ctx.states->attacker_simple_search;

    if (fabs(ctx.cam.ball_angle) <= ctx.params.attacker.orbit_exit_angle)
        return &ctx.states->attacker_simple_capture;

    if (ctx.ball_visible)
    {
        if (ctx.cam.ball_angle < 0)
            cmd.move_angle = ctx.cam.ball_angle - 90;

        if (ctx.cam.ball_angle > 0)
            cmd.move_angle = ctx.cam.ball_angle + 90;
    }

    cmd.align_angle = 0;
    cmd.align_mode = AlignMode::NORMAL;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;

    return nullptr;
}

State *StateAttackerSimpleCapture::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_visible)
        return &ctx.states->attacker_simple_search;

    if (ctx.cam.ball_distance > ctx.params.attacker.orbit_distance)
        return &ctx.states->attacker_simple_approach;

    if (fabs(ctx.cam.ball_angle) > ctx.params.attacker.orbit_enter_angle)
        return &ctx.states->attacker_simple_orbit;

    if (ctx.ball_captured)
        return &ctx.states->attacker_simple_drive;

    cmd.align_angle = 0;
    cmd.align_mode = AlignMode::NORMAL;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.ball_angle;

    return nullptr;
}

State *StateAttackerSimpleDrive::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.ball_captured)
        return &ctx.states->attacker_simple_search;

    if (ctx.cam.opp_goal_distance <= ctx.params.attacker.kick_goal_distance)
        return &ctx.states->attacker_simple_kick;

    cmd.align_angle = 0;
    cmd.move_angle = 0;
    cmd.align_mode = AlignMode::BALL_HOLD;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;

    return nullptr;
}

void StateAttackerSimpleKick::enter(StateContext &ctx)
{
    _timer.set_time_ms(ctx.params.attacker.kick_cooldown_ms);
    _fired = false;
}

State *StateAttackerSimpleKick::update(StateContext &ctx, Command &cmd)
{
    if (_timer.time_is_up())
        return &ctx.states->attacker_simple_search;

    cmd.align_angle = 0;
    cmd.move_angle = 0;

    if (!_fired)
    {
        cmd.kick = true;
        _fired = true;
    }

    cmd.align_mode = AlignMode::BALL_HOLD;
    cmd.dribble = true;
    cmd.lin_speed = ctx.params.speed;
    cmd.stop = false;

    return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------- DEFENDER ------------------------------------------------------ //

State *StateDefenderReturn::update(StateContext &ctx, Command &cmd)
{
    if (ctx.cam.own_goal_distance <= ctx.params.defender.guard_distance)
        return &ctx.states->defender_guard_front;

    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
    cmd.lin_speed = ctx.params.speed;
    cmd.move_angle = ctx.cam.own_goal_angle;
    return nullptr;
}
/*
Guard zone around the own goal, all relative to the goal center:

             ball
                \
     LEFT        \  FRONT        RIGHT
                  \
         +---------x---------+        y = guard_line_distance, x from -guard_half_width to +guard_half_width
        /           \         \
       /             \         \      side lines at guard_side_angle to the goal line
  ----/---------------O---------\---- goal line, O = own goal center

The robot stands where the goal -> ball line crosses the zone border, so it is always between the ball and the goal
*/

enum class GuardSide : int8_t
{
    LEFT = -1,
    FRONT = 0,
    RIGHT = 1
};

// The goal -> ball line is y = kx + b, or x = const when the ball is straight in front of the goal
static bool ball_line_vertical(StateContext &ctx) { return fabs(ctx.ball.x - ctx.own_goal.x) < 0.01; }

static LinearFunction ball_line(StateContext &ctx)
{
    float k = (ctx.ball.y - ctx.own_goal.y) / (ctx.ball.x - ctx.own_goal.x);
    return LinearFunction(k, ctx.own_goal.y - k * ctx.own_goal.x);
}

// x (relative to the goal center) where the goal -> ball line crosses the front line, continued to both sides.
// The ball is level with the goal or behind it: the line never reaches the front line, it goes off to the side
static float front_crossing_x(StateContext &ctx)
{
    if (ctx.ball.y <= ctx.own_goal.y)
        return ctx.ball.x >= ctx.own_goal.x ? INFINITY : -INFINITY;
    if (ball_line_vertical(ctx))
        return 0;

    FunctionIntersector intersector;
    LinearFunction front_line(0, ctx.own_goal.y + ctx.params.defender.guard_line_distance);
    return intersector.intersect(ball_line(ctx), front_line).x - ctx.own_goal.x;
}

// Which part of the border to guard
static GuardSide guard_side(StateContext &ctx)
{
    if (!ctx.ball_visible)
        return GuardSide::FRONT;

    float x = front_crossing_x(ctx);
    if (x > ctx.params.defender.guard_half_width)
        return GuardSide::RIGHT;
    if (x < -ctx.params.defender.guard_half_width)
        return GuardSide::LEFT;
    return GuardSide::FRONT;
}

static State *guard_state(StateContext &ctx, GuardSide side)
{
    switch (side)
    {
    case GuardSide::LEFT:
        return &ctx.states->defender_guard_left;
    case GuardSide::RIGHT:
        return &ctx.states->defender_guard_right;
    default:
        return &ctx.states->defender_guard_front;
    }
}

// Point on the front line where the goal -> ball line crosses it, kept between its ends
static point guard_front_point(StateContext &ctx)
{
    float half = ctx.params.defender.guard_half_width;
    float x = ctx.ball_visible ? constrain(front_crossing_x(ctx), -half, half) : 0; // no ball: the middle of the line
    return {ctx.own_goal.x + x, ctx.own_goal.y + ctx.params.defender.guard_line_distance};
}

// Point on a side line (side = LEFT or RIGHT) where the goal -> ball line crosses it, kept between the end of the
// front line and the goal line
static point guard_side_point(StateContext &ctx, GuardSide side)
{
    float s = static_cast<int>(side);
    float front_y = ctx.own_goal.y + ctx.params.defender.guard_line_distance;
    point corner = {ctx.own_goal.x + s * ctx.params.defender.guard_half_width, front_y};
    if (!ctx.ball_visible)
        return corner;

    // The side line goes from the corner outwards and back to the goal line: y = kx + b, x = const at 90 deg
    float angle = radians(ctx.params.defender.guard_side_angle);
    bool side_vertical = fabs(cos(angle)) < 0.001;
    float k = side_vertical ? 0 : -s * tan(angle);
    LinearFunction side_line(k, corner.y - k * corner.x);
    VerticalLine side_vertical_line(corner.x);

    FunctionIntersector intersector;
    point p;
    if (ball_line_vertical(ctx))
    {
        if (side_vertical)
            return corner; // parallel
        p = intersector.intersect(VerticalLine(ctx.own_goal.x), side_line);
    }
    else
    {
        LinearFunction ball = ball_line(ctx);
        if (side_vertical)
            p = intersector.intersect(ball, side_vertical_line);
        else if (ball.k_coef() == side_line.k_coef())
            return corner; // parallel
        else
            p = intersector.intersect(ball, side_line);
    }

    // The lines cross on the other side of the goal, not between the goal and the ball
    if (dot(MathVector(ctx.own_goal, p), MathVector(ctx.own_goal, ctx.ball)) < 0)
        return corner;

    // Only the part from the corner to the goal line belongs to the zone
    float y = constrain(p.y, ctx.own_goal.y, front_y);
    if (y == p.y)
        return p;
    return {side_vertical ? corner.x : (y - side_line.b_koef()) / k, y};
}

// Drives to the guard point and slows down near it, otherwise the robot overshoots and swings around it.
// At the point it stands still until the point (or the robot) moves guard_hold_distance away
void StateDefenderGuard::_guard(StateContext &ctx, Command &cmd, point target)
{
    // The camera sends angles in 3 deg and distances in 3 cm steps, so the point jumps by several cm on a still field.
    // Follow it only partly every frame: single jumps are smoothed out, a real move of the ball still comes through
    if (!_started)
    {
        _target = target;
        _holding = false;
        _started = true;
    }
    float k = ctx.params.defender.guard_smoothing;
    _target = {_target.x + k * (target.x - _target.x), _target.y + k * (target.y - _target.y)};
    target = _target;

    float distance = MathVector(ctx.coords, target).length();
    if (_holding && distance > ctx.params.defender.guard_hold_distance)
        _holding = false;
    else if (!_holding && distance < ctx.params.defender.guard_arrive_distance)
        _holding = true;

    move_to(ctx, cmd, target);
    cmd.lin_speed = _holding ? 0 : ctx.params.speed * min(1.0f, distance / ctx.params.defender.guard_slow_distance);
    cmd.align_angle = 0;
    cmd.dribble = false;
    cmd.kick = false;
}

State *StateDefenderGuardFront::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.own_goal_known)
        return &ctx.states->defender_return;

    GuardSide side = guard_side(ctx);
    if (side != GuardSide::FRONT)
        return guard_state(ctx, side);

    _guard(ctx, cmd, guard_front_point(ctx));
    return nullptr;
}

State *StateDefenderGuardLeft::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.own_goal_known)
        return &ctx.states->defender_return;

    GuardSide side = guard_side(ctx);
    if (side != GuardSide::LEFT)
        return guard_state(ctx, side);

    _guard(ctx, cmd, guard_side_point(ctx, GuardSide::LEFT));
    return nullptr;
}

State *StateDefenderGuardRight::update(StateContext &ctx, Command &cmd)
{
    if (!ctx.own_goal_known)
        return &ctx.states->defender_return;

    GuardSide side = guard_side(ctx);
    if (side != GuardSide::RIGHT)
        return guard_state(ctx, side);

    _guard(ctx, cmd, guard_side_point(ctx, GuardSide::RIGHT));
    return nullptr;
}
