#include <Arduino.h>

#include "core/sensors/button/button.h"
#include "core/led/led.h"

#include "control/point-navigator/point-navigator.h"
#include "control/heading-controller/heading-controller.h"
#include "control/ball-coordinates-calculator/ball-coordinates-calculator.h"
#include "control/camera-data-parser/camera-data-parser.h"

#include "management/robot-type/robot-type.h"
#include "management/config/robot-config.h"

#define SPEED 150
#define TARGET_POINT {0, -40}
#define GO_TO_BALL false

class PointNavigatorTest
{
public:
    PointNavigatorTest(MovementController mc, HeadingController align, BNO055 gyro, CameraDataParser cd, Button button, Led led) : _mc(mc), _align(align), _gyro(gyro), _cd(cd), _button(button), _led(led) {}
    void setup()
    {
        _cd.init();
        _mc.init();
        _gyro.init();
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
    void loop(bool go_to_ball, point target_point, int speed)
    {
        _gyro.read();
        Serial.println("Gyro read");
        _cd.parse();
        Serial.println("Camera data parsed");

        _rc.calculate_coordinates(_gyro.data(), _cd.data());

        point target = target_point;

        if (go_to_ball)
        {
            _bc.calculate(_gyro.data(), _cd.data(), _rc.coordinates());
            target = _bc.coordinates();
        }

        _pn.compute(_rc.coordinates(), target);

        _mc.set_velocity(speed, _pn.data().move_angle - _gyro.data().yaw, _align.calculate(0, _gyro.data()));
        _mc.move();

        Serial.print("Move angle:\t");
        Serial.println(_pn.data().move_angle);
    }

private:
    PointNavigator _pn;
    MovementController _mc;
    HeadingController _align;
    BNO055 _gyro;
    RobotCoordinatesCalculator _rc;
    BallCoordinatesCalculator _bc;
    CameraDataParser _cd;
    Button _button;
    Led _led;
};

RobotWhite robot_type;
RobotConfig config(robot_type);
GoalColor own_goal = GoalColor::BLUE;

MovementController mc(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles);
HeadingController align(new PDCControl(config.controls_config().pdc));
BNO055 gyro(config.gyro_address());
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
CameraDataParser cd(uart, GoalMapper(own_goal));
Button button(config.pins().btn1);
Led led(config.pins().led);

PointNavigatorTest test(mc, align, gyro, cd, button, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(GO_TO_BALL, TARGET_POINT, SPEED);
}
