#include "BNO055.h"

BNO055::BNO055(uint8_t IIC_address) : _bno(Adafruit_BNO055(55, IIC_address))
{
    _data.pitch = 0;
    _data.roll = 0;
    _data.yaw = 0;

    _zero_angles.pitch = 0;
    _zero_angles.roll = 0;
    _zero_angles.yaw = 0;
}

int16_t BNO055::_wrap_angle(int16_t angle)
{
    if (angle > 180)
        return angle - 360;
    else if (angle < -180)
        return angle + 360;
    else
        return angle;
}

void BNO055::init()
{
    if (!_bno.begin(adafruit_bno055_opmode_t::OPERATION_MODE_IMUPLUS))
    {
        Serial.println("Gyroscope: BNO055: NO CONNECTION");
        while (true)
            ;
    }

    _bno.setExtCrystalUse(true);
    _bno.setMode(adafruit_bno055_opmode_t::OPERATION_MODE_IMUPLUS);

    if (_bno.getMode() != adafruit_bno055_opmode_t::OPERATION_MODE_IMUPLUS)
    {
        _bno.setMode(adafruit_bno055_opmode_t::OPERATION_MODE_IMUPLUS);
    }
}

bool BNO055::calibrated()
{
    uint8_t system, gyro, accel, mag;
    _bno.getCalibration(&system, &gyro, &accel, &mag);
    return (gyro == 3);
}

void BNO055::read()
{
    sensors_event_t event;
    _bno.getEvent(&event);
    _data.yaw = _wrap_angle(event.orientation.x - _zero_angles.yaw);
    _data.roll = _wrap_angle(event.orientation.y - _zero_angles.roll);
    _data.pitch = _wrap_angle(event.orientation.z - _zero_angles.pitch);
}

void BNO055::set_zero_angles()
{
    sensors_event_t event;
    _bno.getEvent(&event);
    _zero_angles.pitch = event.orientation.z;
    _zero_angles.roll = event.orientation.y;
    _zero_angles.yaw = event.orientation.x;
}

gyroData BNO055::data() { return _data; }

bnoStatus BNO055::status()
{
    bnoStatus result;
    result.mode = (uint8_t)_bno.getMode();
    _bno.getSystemStatus(&result.system, &result.self_test, &result.error);
    return result;
}
