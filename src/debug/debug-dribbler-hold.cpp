#include <Arduino.h>
#include "config.h"
#include "control/movement-controller/movement-controller.h"
#include "control/heading-controller/heading-controller.h"
#include "control/ball-detector/ball-detector.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "core/dribbler/dribbler.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"
#include "management/goal-mapper/goal-mapper.h"

// Checks how the dribbler holds the ball: the dribbler spins all the time and the robot turns to a heading.
// Put the ball into the dribbler and press the button: the terminal shows how long the ball is held and how often it's lost.
// The button pauses and resumes the test (motors and dribbler stop).

#define ALIGN_OPP_GOAL 0 // turn to the opponent goal (camera)
#define ALIGN_OWN_GOAL 1 // turn to the own goal (camera)
#define ALIGN_FIXED 2    // keep FIXED_ANGLE (gyro, 0 = the heading at the zero angles setup)
#define ALIGN_SWEEP 3    // turn to SWEEP_ANGLES one after another: the hardest test, the ball is lost on turns

#define ALIGN_MODE ALIGN_OWN_GOAL

#define FIXED_ANGLE 90
const int SWEEP_ANGLES[] = {0, 90, 180, -90};
#define SWEEP_STEP_MS 2000

// 0 to turn on the spot, otherwise the robot also drives with this speed and direction (relative to its front)
#define SPEED 0
#define MOVE_ANGLE 0

#define PRINT_EVERY_MS 100

class DribblerHoldTest
{
public:
    DribblerHoldTest(MovementController mc, HeadingController align, BNO055 gyro, Button button, CameraDataParser cam_parser, Led led, Dribbler dribbler, BallDetector ball_detector)
        : _m(mc), _align(align), _gyro(gyro), _button(button), _cam_parser(cam_parser), _led(led), _dribbler(dribbler), _ball_detector(ball_detector) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
        _m.init();
        _button.init();
        _led.init();
        _dribbler.init();
        _ball_detector.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to calibrate the gyroscope.");
        _button.wait_click();

        while (!_gyro.calibrated())
            ;
        _led.on();

        Serial.println("Gyroscope calibrated");
        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to set the zero angles.");
        _button.wait_click();
        _gyro.set_zero_angles();

        Serial.print("Put the ball into the dribbler and press button #");
        Serial.print(_button.pin());
        Serial.println(" to start. The same button pauses the test.");
        _button.wait_click();

        _start();
    }
    void loop(int lin_speed, int move_angle)
    {
        _gyro.read();
        _ball_detector.read();
        _button.read();

        // Pause / resume on a click (on release, so one click doesn't toggle it every loop)
        bool pressed = _button.is_pressed();
        if (_was_pressed && !pressed)
        {
            _paused = !_paused;
            if (_paused)
            {
                _m.stop();
                _dribbler.stop();
                Serial.println("Paused");
            }
            else
                _start();
        }
        _was_pressed = pressed;
        if (_paused)
            return;

        _update_target();
        _update_ball();

        _dribbler.run();
        _m.set_velocity(lin_speed, move_angle, _align.calculate(_target, _gyro.data()));
        _m.move();

        if (millis() - _last_print >= PRINT_EVERY_MS)
        {
            _last_print = millis();
            _print();
        }
    }

private:
    MovementController _m;
    HeadingController _align;
    BNO055 _gyro;
    Button _button;
    CameraDataParser _cam_parser;
    Led _led;
    Dribbler _dribbler;
    BallDetector _ball_detector;

    int _target = 0;
    bool _paused = false;
    bool _was_pressed = false;
    unsigned long _sweep_mark = 0;
    int _sweep_i = 0;
    unsigned long _last_print = 0;

    bool _ball = false;
    unsigned long _captured_at = 0;
    unsigned long _longest_hold = 0;
    unsigned int _losses = 0;

    void _start()
    {
        _sweep_mark = millis();
        _sweep_i = 0;
        _ball = false;
        _losses = 0;
        _longest_hold = 0;
        Serial.println("Started");
    }

    void _update_target()
    {
        if (ALIGN_MODE == ALIGN_FIXED)
            _target = FIXED_ANGLE;
        else if (ALIGN_MODE == ALIGN_SWEEP)
        {
            const int n = sizeof(SWEEP_ANGLES) / sizeof(SWEEP_ANGLES[0]);
            if (millis() - _sweep_mark >= SWEEP_STEP_MS)
            {
                _sweep_mark = millis();
                _sweep_i = (_sweep_i + 1) % n;
            }
            _target = SWEEP_ANGLES[_sweep_i];
        }
        else
        {
            _cam_parser.parse();
            cameraData cam = _cam_parser.data();
            bool opp = ALIGN_MODE == ALIGN_OPP_GOAL;
            int goal_angle = opp ? cam.opp_goal_angle : cam.own_goal_angle;
            int goal_distance = opp ? cam.opp_goal_distance : cam.own_goal_distance;

            // The camera reports 0 for a goal it doesn't see: then keep turning to the last known heading
            if (goal_distance > 0)
                _target = _wrap(_gyro.data().yaw + goal_angle);
        }
    }

    void _update_ball()
    {
        bool ball = _ball_detector.ball_detected();
        if (ball && !_ball)
            _captured_at = millis();
        if (!ball && _ball)
        {
            _losses++;
            Serial.print(">>> Ball lost after ");
            Serial.print(millis() - _captured_at);
            Serial.print(" ms, yaw ");
            Serial.print(_gyro.data().yaw);
            Serial.print(", target ");
            Serial.println(_target);
        }
        if (ball)
            _longest_hold = max(_longest_hold, millis() - _captured_at);
        _ball = ball;
    }

    void _print()
    {
        Serial.print("ball: ");
        Serial.print(_ball ? "HELD " : "---  ");
        Serial.print(_ball ? (millis() - _captured_at) : 0);
        Serial.print(" ms\tlongest: ");
        Serial.print(_longest_hold);
        Serial.print(" ms\tlosses: ");
        Serial.print(_losses);
        Serial.print("\tphoto: ");
        Serial.print(_ball_detector.phototransistor_data());
        Serial.print("\t|\tyaw: ");
        Serial.print(_gyro.data().yaw);
        Serial.print("\ttarget: ");
        Serial.println(_target);
    }

    static int _wrap(int angle)
    {
        while (angle > 180)
            angle -= 360;
        while (angle < -180)
            angle += 360;
        return angle;
    }
};

RobotViolet robot_type;
RobotConfig config(robot_type);

MovementController movement_controller(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles);
// The regulator for turning with the ball: tune with_ball_align_pdc / with_ball_align_max_speed in config.json with this test
HeadingController align(new PDCControl(config.controls_config().with_ball_pdc), config.controls_config().with_ball_max_speed);
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
Led led(config.pins().led);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalColor own_goal = GoalColor::BLUE;
CameraDataParser cam_parser(uart, GoalMapper(own_goal));
Dribbler dribbler(config.pins().dribbler, config.dribbler_config().min, config.dribbler_config().max);
BallDetector ball_detector(Phototransistor(config.pins().phototransistor), config.phototransistor_threshold());

DribblerHoldTest test(movement_controller, align, gyro, button, cam_parser, led, dribbler, ball_detector);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(SPEED, MOVE_ANGLE);
}
