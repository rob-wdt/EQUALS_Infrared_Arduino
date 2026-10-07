#include <Arduino.h>
#include "config.h"
#include "core/UART/UART.h"
#include "management/config/robot-config.h"

#define UART_BUFFER_SIZE 5

class CamPerceptionTest
{
public:
    CamPerceptionTest(UART uart) : _uart(uart) {}
    void setup()
    {
        Serial.begin(115200);
        _uart.init();
    }
    void loop()
    {
        _uart.receive();

        Serial.print("Received data:\t");
        for (int i = 0; i < _uart.buffer_size(); i++)
        {
            Serial.print(_uart.recieved_data()[i]);
            Serial.print("\t");
        }
        Serial.println();
    }

private:
    UART _uart;
};

RobotViolet robot_type;
RobotConfig config(robot_type);

UART uart(config.UART_config().num, config.UART_config().baudrate, UART_BUFFER_SIZE, config.UART_config().buffer_starter);
CamPerceptionTest test(uart);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
