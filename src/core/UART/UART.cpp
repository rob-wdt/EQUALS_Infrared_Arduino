#include "UART.h"

UART::UART(UARTNum uart_num, uint32_t baud_rate, unsigned buffer_size, uint8_t buffer_starter) : _uart_num(uart_num), _baud_rate(baud_rate), _buffer_size(buffer_size), _buffer_starter(buffer_starter)
{
    for (unsigned i = 0; i < MAX_BUFFER_SIZE; i++)
        _recieved_data[i] = 0;
}

int UART::_available()
{
    switch (_uart_num)
    {
    case UARTNum::UART_1:
        return Serial1.available();
    case UARTNum::UART_2:
        return Serial2.available();
    case UARTNum::UART_3:
        return Serial3.available();
    }

    return 0;
}

int UART::_read_byte()
{
    switch (_uart_num)
    {
    case UARTNum::UART_1:
        return Serial1.read();
    case UARTNum::UART_2:
        return Serial2.read();
    case UARTNum::UART_3:
        return Serial3.read();
    }

    return -1;
}

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
    while (_available() < _buffer_size)
        ;

    int first_byte = _read_byte();

    if (first_byte == _buffer_starter)
    {
        _recieved_data[0] = first_byte;
        for (int i = 1; i < _buffer_size; i++)
        {
            _recieved_data[i] = _read_byte();
        }
    }
}

const int *UART::recieved_data() const { return _recieved_data; }

unsigned UART::buffer_size() const { return _buffer_size; }
