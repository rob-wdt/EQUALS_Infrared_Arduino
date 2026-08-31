#include "UART.h"

UART::UART(UARTNum uart_num, uint32_t baud_rate, unsigned buffer_size, uint8_t buffer_starter) : _uart_num(uart_num), _baud_rate(baud_rate), _buffer_size(buffer_size), _buffer_starter(buffer_starter) {}

void UART::init()
{
    switch (_uart_num)
    {
    case UARTNum::UART_1:
        Serial1.begin(_baud_rate);
        break;
    case UARTNum::UART_2:
        Serial2.begin(_baud_rate);
        break;
    case UARTNum::UART_3:
        Serial3.begin(_baud_rate);
        break;
    }
}

void UART::send(const String &data)
{
    switch (_uart_num)
    {
    case UARTNum::UART_1:
        Serial1.write(data.c_str());
        break;
    case UARTNum::UART_2:
        Serial2.write(data.c_str());
        break;
    case UARTNum::UART_3:
        Serial3.write(data.c_str());
        break;
    }
}

void UART::receive()
{
    unsigned available;

    switch (_uart_num)
    {
    case UARTNum::UART_1:
        available = Serial1.available();
        break;
    case UARTNum::UART_2:
        available = Serial2.available();
        break;
    case UARTNum::UART_3:
        available = Serial3.available();
        break;
    }

    Serial.print("Available: ");
    Serial.println(available);

    if (available >= _buffer_size)
    {
        uint8_t first_byte;

        switch (_uart_num)
        {
        case (UARTNum::UART_1):
            first_byte = Serial1.read();
            break;
        case (UARTNum::UART_2):
            first_byte = Serial2.read();
            break;
        case (UARTNum::UART_3):
            first_byte = Serial3.read();
            break;
        }

        if (first_byte == _buffer_starter)
        {
            _recieved_data[0] = first_byte;

            for (int i = 0; i < _buffer_size - 1; i++)
            {
                switch (_uart_num)
                {
                case (UARTNum::UART_1):
                    _recieved_data[i + 1] = Serial1.read();
                    break;
                case (UARTNum::UART_2):
                    _recieved_data[i + 1] = Serial2.read();
                    break;
                case (UARTNum::UART_3):
                    _recieved_data[i + 1] = Serial3.read();
                    break;
                }
            }
        }
    }
}

uint8_t *UART::recieved_data() const { return _recieved_data; }

unsigned UART::buffer_size() const { return _buffer_size; }
