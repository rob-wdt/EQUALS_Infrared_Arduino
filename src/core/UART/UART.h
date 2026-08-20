#pragma once

#include <Arduino.h>
#include "core/device.h"

enum UART_NUM
{
    UART_1 = 1,
    UART_2 = 2,
    UART_3 = 3
};

class UART : public Device
{
public:
    UART(UART_NUM uart_num, uint32_t baud_rate);
    void init() override;
    void send(const String &data);
    void receive();
    String recieved_data();

private:
    UART_NUM _uart_num;
    uint32_t _baud_rate;
    String _recieved_data;
};
