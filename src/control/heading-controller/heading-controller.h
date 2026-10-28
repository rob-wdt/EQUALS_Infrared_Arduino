#pragma once

#include "core/sensors/BNO055/BNO055.h"
#include "utils/control/control.h"

// Turns a target heading into the angular speed for MovementController::set_velocity()
class HeadingController
{
public:
    // max_speed: PWM, the angular speed is limited to +-max_speed (255 = no limit)
    HeadingController(Control *control, float max_speed = 255);
    // target_heading: heading in the gyro frame (0 = the direction set at calibration)
    float calculate(int target_heading, gyroData gyro_data);

private:
    Control *_control;
    float _max_speed;
};
