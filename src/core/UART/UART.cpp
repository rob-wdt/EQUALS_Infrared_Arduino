#include "UART.h"

UART::UART(UART_NUM uart_num, uint32_t baud_rate) : _uart_num(uart_num), _baud_rate(baud_rate) {}

void UART::init()
{
    switch (_uart_num)
    {
    case UART_NUM::UART_1:
        Serial1.begin(_baud_rate);
        break;
    case UART_NUM::UART_2:
        Serial2.begin(_baud_rate);
        break;
    case UART_NUM::UART_3:
        Serial3.begin(_baud_rate);
        break;
    }
}

void UART::send(const String &data)
{
    switch (_uart_num)
    {
    case UART_NUM::UART_1:
        Serial1.write(data.c_str());
        break;
    case UART_NUM::UART_2:
        Serial2.write(data.c_str());
        break;
    case UART_NUM::UART_3:
        Serial3.write(data.c_str());
        break;
    }
}

void UART::receive()
{
    _recieved_data = ""; // Clear previous data

    switch (_uart_num)
    {
    case UART_NUM::UART_1:
        while (Serial1.available())
        {
            _recieved_data += (char)Serial1.read();
        }
        break;
    case UART_NUM::UART_2:
        while (Serial2.available())
        {
            _recieved_data += (char)Serial2.read();
        }
        break;
    case UART_NUM::UART_3:
        while (Serial3.available())
        {
            _recieved_data += (char)Serial3.read();
        }
        break;
    }
}

String UART::recieved_data() { return _recieved_data; }
