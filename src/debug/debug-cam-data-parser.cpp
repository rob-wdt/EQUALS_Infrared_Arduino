#include <Arduino.h>
#include "config.h"
#include "core/UART/UART.h"
#include "control/camera-data-parser/camera-data-parser.h"
#include "management/config/robot-config.h"

class CamDataParserTest
{
public:
    CamDataParserTest(UART uart, GoalMapper goal) : _uart(uart), _c(uart, goal) {}
    void setup()
    {
        _c.init();
        Serial.begin(115200);
    }
    void loop()
    {
        _c.parse();
        Serial.print("Parsed data:\tBall angle: ");
        Serial.print(_c.data().ball_angle);
        Serial.print("\t\tBall distance: ");
        Serial.print(_c.data().ball_distance);
        Serial.print("\tOwn goal angle: ");
        Serial.print(_c.data().own_goal_angle);
        Serial.print("\tOwn goal distance: ");
        Serial.print(_c.data().own_goal_distance);
        Serial.print("\t\tOpp goal angle: ");
        Serial.print(_c.data().opp_goal_angle);
        Serial.print("\tOpp goal distance: ");
        Serial.println(_c.data().opp_goal_distance);
    }

private:
    UART _uart;
    CameraDataParser _c;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

UART uart(config.UART_config().num, config.UART_config().baudrate, config.UART_config().buffer_size, config.UART_config().buffer_starter);
GoalMapper goal(GoalColor::YELLOW);
CamDataParserTest test(uart, goal);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
