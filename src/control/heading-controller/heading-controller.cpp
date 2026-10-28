#include "heading-controller.h"
#include <Arduino.h>

HeadingController::HeadingController(Control *control, float max_speed) : _control(control), _max_speed(max_speed) {}

float HeadingController::calculate(int target_heading, gyroData gyro_data)
{
    int16_t error = target_heading - gyro_data.yaw;
    if (error > 180)
        error -= 360;
    else if (error < -180)
        error += 360;

    return constrain(_control->calculate(error), -_max_speed, _max_speed);
}
