#include <Arduino.h>

#include "control/point-navigator/point-navigator.h"
#include "control/ball-coordinates-calculator/ball-coordinates-calculator.h"
#include "control/camera-data-parser/camera-data-parser.h"

#include "management/robot-type/robot-type.h"
#include "management/config/robot-config.h"

#define SPEED 0
#define TARGET_POINT {0, 0}
#define GO_TO_BALL false

class PointNavigatorTest
{
public:
    PointNavigatorTest(MovementController mc, BNO055 gyro, CameraDataParser cd) : _mc(mc), _gyro(gyro), _cd(cd) {}
    void setup()
    {
        _mc.init();
        _gyro.init();
    }
    void loop(bool go_to_ball, coordinates target_point, int speed)
    {
        _gyro.read();
        _cd.parse();

        _rc.calculate_coordinates(_gyro.data(), _cd.data());

        coordinates target = target_point;

        if (go_to_ball)
        {
            _bc.calculate(_gyro.data(), _cd.data(), _rc.coordinates());
            target = _bc.coordinates();
        }

        movementData move_data = {speed, 0, 0};

        _pn.compute(_rc.coordinates(), target, move_data);

        _mc.set_velocity(_pn.data(), _gyro.data());
        _mc.move();
    }

private:
    PointNavigator _pn;
    MovementController _mc;
    BNO055 _gyro;
    RobotCoordinatesCalculator _rc;
    BallCoordinatesCalculator _bc;
    CameraDataParser _cd;
};

RobotViolet robot_type;
RobotConfig config(robot_type);
GoalColor own_goal = GoalColor::BLUE;

MovementController mc(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles,
    new PDCControl(config.controls_config().pdc));
BNO055 gyro(config.gyro_address());
UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
CameraDataParser cd(uart, GoalMapper(own_goal));

PointNavigatorTest test(mc, gyro, cd);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(GO_TO_BALL, TARGET_POINT, SPEED);
}
