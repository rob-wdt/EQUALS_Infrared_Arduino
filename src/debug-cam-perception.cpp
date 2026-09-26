#include <Arduino.h>
#include "config.h"
#include "core/UART/UART.h"

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

UART uart(UARTNum::UART_3, 230400, UART_BUFFER_SIZE, UART_BUFFER_STARTER);
CamPerceptionTest test(uart);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
