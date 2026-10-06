#include "camera-data-parser.h"

CameraDataParser::CameraDataParser(UART uart, GoalMapper goal_mapper) : _uart(uart), _goal(goal_mapper) {}

void CameraDataParser::init() { _uart.init(); }

void CameraDataParser::parse()
{
    _uart.receive();
    auto recieved_raw = _uart.recieved_data();

    rawCameraData raw = {
        (recieved_raw[5] - 127) * 3,
        (recieved_raw[6] - 127) * 3,
        (recieved_raw[3] - 127) * 3,
        (recieved_raw[4] - 127) * 3,
        (recieved_raw[1] - 127) * 3,
        (recieved_raw[2] - 127) * 3};

    _goal.map(raw);

    _data = _goal.data();
}

cameraData CameraDataParser::data() { return _data; }
