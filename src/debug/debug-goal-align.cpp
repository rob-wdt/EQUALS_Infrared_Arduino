#include <Arduino.h>
#include "config.h"
#include "control/movement-controller/movement-controller.h"
#include "control/heading-controller/heading-controller.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"
#include "management/goal-mapper/goal-mapper.h"
#include "debug/debug-controls.h"

// true: turn to the opponent goal, false: turn to the own goal
#define ALIGN_ON_OPP_GOAL true

#define SPEED 150
#define MOVE_ANGLE 90

class GoalAlignTest
{
public:
    GoalAlignTest(MovementController mc, HeadingController align, BNO055 gyro, DebugControls controls, CameraDataParser cam_parser, Led led) : _m(mc), _align(align), _gyro(gyro), _controls(controls), _cam_parser(cam_parser), _led(led) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
        _m.init();
        _controls.init();
        _led.init();
        Serial.begin(115200);

        _controls.calibrate_and_set_zero(_gyro, _led);
        _controls.wait_start();
    }
    void loop(float lin_speed, float move_angle)
    {
        if (!_controls.update(_gyro, &_cam_parser))
        {
            _m.stop();
            return;
        }
        _gyro.read();
        _cam_parser.parse();

        cameraData cam = _cam_parser.data();
        int goal_angle = ALIGN_ON_OPP_GOAL ? cam.opp_goal_angle : cam.own_goal_angle;
        int goal_distance = ALIGN_ON_OPP_GOAL ? cam.opp_goal_distance : cam.own_goal_distance;

        // The camera reports 0 for a goal it doesn't see: then keep turning to the last known heading
        if (goal_distance > 0)
        {
            // Goal angle relative to the robot front, then the heading at which the goal is straight ahead
            int to_goal = _wrap(goal_angle);
            _target = _wrap(_gyro.data().yaw + to_goal);
        }

        _m.set_velocity(lin_speed, move_angle - _gyro.data().yaw, _align.calculate(_target, _gyro.data()));
        _m.move();

        Serial.print("Goal angle: ");
        Serial.print(goal_angle);
        Serial.print("\tdist: ");
        Serial.print(goal_distance);
        Serial.print("\t|\tyaw: ");
        Serial.print(_gyro.data().yaw);
        Serial.print("\ttarget: ");
        Serial.println(_target);
    }

private:
    MovementController _m;
    HeadingController _align;
    BNO055 _gyro;
    DebugControls _controls;
    CameraDataParser _cam_parser;
    Led _led;

    int _target = 0;

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
HeadingController align(new PDCControl(config.controls_config().pdc));
BNO055 gyro(config.gyro_address());
DebugControls controls(config.pins().btn1, config.pins().btn2, config.pins().btn3);
Led led(config.pins().led1);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalColor own_goal = GoalColor::BLUE;
CameraDataParser cam_parser(uart, GoalMapper(own_goal));

GoalAlignTest test(movement_controller, align, gyro, controls, cam_parser, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(SPEED, MOVE_ANGLE);
}
