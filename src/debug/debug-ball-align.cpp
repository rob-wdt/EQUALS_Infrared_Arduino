#include <Arduino.h>
#include "config.h"
#include "control/movement-controller/movement-controller.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/sensors/button/button.h"
#include "core/led/led.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"
#include "management/goal-mapper/goal-mapper.h"

// CV.py changes the sign of the ball angle only (ball.angle = -ball.angle), so to compare it with the gyro yaw
// it has to be turned back. If the robot turns away from the ball, set 1
#define BALL_ANGLE_SIGN 1

// 0 to turn on the spot, otherwise the robot drives with this speed and direction while keeping the ball in front
#define SPEED 150
#define MOVE_ANGLE 0

class BallAlignTest
{
public:
    BallAlignTest(MovementController mc, BNO055 gyro, Button button, CameraDataParser cam_parser, Led led) : _m(mc), _gyro(gyro), _button(button), _cam_parser(cam_parser), _led(led) {}
    void setup()
    {
        _gyro.init();
        _cam_parser.init();
        _m.init();
        _button.init();
        _led.init();
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

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to start.");
        _button.wait_click();
    }
    void loop(float lin_speed, float move_angle)
    {
        _gyro.read();
        _cam_parser.parse();

        cameraData cam = _cam_parser.data();

        // The camera reports 0 for a ball it doesn't see: then keep turning to the last known heading
        if (cam.ball_distance > 0)
        {
            // Ball angle relative to the robot front, then the heading at which the ball is straight ahead
            int to_ball = _wrap(BALL_ANGLE_SIGN * cam.ball_angle);
            _target = _wrap(_gyro.data().yaw + to_ball);
        }

        _m.set_velocity(lin_speed, move_angle, _target, _gyro.data());
        _m.move();

        Serial.print("Ball angle: ");
        Serial.print(cam.ball_angle);
        Serial.print("\tdist: ");
        Serial.print(cam.ball_distance);
        Serial.print("\t|\tyaw: ");
        Serial.print(_gyro.data().yaw);
        Serial.print("\ttarget: ");
        Serial.println(_target);
    }

private:
    MovementController _m;
    BNO055 _gyro;
    Button _button;
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
    config.motors_config().loc_angles,
    new PDCControl(config.controls_config().pdc));
BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
Led led(config.pins().led);
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalColor own_goal = GoalColor::BLUE;
CameraDataParser cam_parser(uart, GoalMapper(own_goal));

BallAlignTest test(movement_controller, gyro, button, cam_parser, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(SPEED, MOVE_ANGLE);
}
