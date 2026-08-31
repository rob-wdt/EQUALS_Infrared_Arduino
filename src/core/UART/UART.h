#pragma once

#include <Arduino.h>
#include "core/device.h"

enum UARTNum
{
    UART_1 = 1,
    UART_2 = 2,
    UART_3 = 3
};

class UART : public Device
{
public:
    UART(UARTNum uart_num, uint32_t baud_rate, unsigned buffer_size, uint8_t buffer_starter);
    void init() override;
    void send(const String &data);
    void receive();
    uint8_t *recieved_data() const;
    unsigned buffer_size() const;

private:
    UARTNum _uart_num;
    uint32_t _baud_rate;
    unsigned _buffer_size;
    uint8_t _buffer_starter;

    uint8_t *_recieved_data;
};
