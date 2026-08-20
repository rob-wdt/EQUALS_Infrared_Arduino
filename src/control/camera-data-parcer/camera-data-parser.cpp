#include "camera-data-parser.h"

CameraDataParser::CameraDataParser(UART uart) : _uart(uart) {}

void CameraDataParser::init() { _uart.init(); }

void CameraDataParser::parse()
{
    _uart.receive();
    String data = _uart.recieved_data();

    char sep = ',';
    int parced_data[6] = {};
    uint8_t index = 0;

    String word = "";
    for (auto i : data)
    {
        if (i != sep)
        {
            word += i;
        }
        else
        {
            parced_data[index] = word.toInt();
            index++;
            word = "";
        }
    }
    parced_data[index] = word.toInt();

    _data = {parced_data[0], parced_data[1], parced_data[2], parced_data[3], parced_data[4], parced_data[5]};
}

cameraData CameraDataParser::data() { return _data; }
