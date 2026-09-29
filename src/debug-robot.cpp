#include <Arduino.h>
#include "management/robot/robot.h"

class RobotTest
{
public:
    RobotTest(Robot robot) : _robot(robot) {}
    void setup()
    {
        _robot.init();
        _robot.calibrate_gyro();
    }
    void loop()
    {
        _robot.update();
    }

private:
    Robot _robot;
};

BallDetector ball_detector(Phototransistor(Pin::PIN_PHOTOTRANSISTOR), 1);
OutsHandler outs_handler;
CoordinatesCalculator coords;
CameraDataParser cam_parser(UART(UARTNum::UART_3, 230400, UART_BUFFER_SIZE, UART_BUFFER_STARTER));
MovementController movement_controller(
    {Motor(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir),
     Motor(Pin::PIN_M2_1, Pin::PIN_M2_2, motors_directions.m2_dir),
     Motor(Pin::PIN_M3_1, Pin::PIN_M3_2, motors_directions.m3_dir),
     Motor(Pin::PIN_M4_1, Pin::PIN_M4_2, motors_directions.m4_dir)},
    motors_loc_angles,
    new PIDControl(control_gyro_align_pid));
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

Robot robot(new RoleDefender(0.5), ball_detector, outs_handler, coords, cam_parser, movement_controller, gyro, button);

RobotTest test(robot);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
