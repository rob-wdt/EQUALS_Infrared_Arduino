#include "camera-data-parser.h"

CameraDataParser::CameraDataParser(UART uart) : _uart(uart) {}

void CameraDataParser::init() { _uart.init(); }

void CameraDataParser::parse()
{
    _uart.receive();
    auto data = _uart.recieved_data();

    _data = {data[1], data[2], data[3], data[4], data[5], data[6]};
}

cameraData CameraDataParser::data() { return _data; }
