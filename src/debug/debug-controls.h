#pragma once

#include <Arduino.h>

#include "core/sensors/button/button.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/led/led.h"
#include "control/camera-data-parser/camera-data-parser.h"

// Shared button layout for debug environments:
// 1 - start/stop, 2 - gyro calibration + zero, 3 - own-goal color.
class DebugControls
{
public:
    DebugControls(uint8_t start_stop_pin, uint8_t gyro_pin, uint8_t goal_pin)
        : _start_stop(start_stop_pin), _gyro_button(gyro_pin), _goal_button(goal_pin) {}

    void init()
    {
        _start_stop.init();
        _gyro_button.init();
        _goal_button.init();
    }

    void calibrate_and_set_zero(BNO055 &gyro, Led &led)
    {
        Serial.print("Press button #");
        Serial.print(_gyro_button.pin());
        Serial.println(" to calibrate the gyroscope and set zero angles.");
        _gyro_button.wait_click();

        while (!gyro.calibrated())
            ;
        gyro.set_zero_angles();
        led.on();
        Serial.println("Gyroscope calibrated; zero angles set");
    }

    void wait_start()
    {
        Serial.print("Press button #");
        Serial.print(_start_stop.pin());
        Serial.println(" to start. Press it again to stop.");
        _start_stop.wait_click();
        _running = true;
    }

    bool update(BNO055 &gyro, CameraDataParser *camera = nullptr)
    {
        _start_stop.read();
        _gyro_button.read();
        _goal_button.read();

        if (_start_stop.was_pressed())
        {
            _running = !_running;
            Serial.println(_running ? "Started" : "Stopped");
        }

        if (_gyro_button.was_pressed())
        {
            _running = false;
            while (!gyro.calibrated())
                ;
            gyro.set_zero_angles();
            Serial.println("Gyroscope calibrated; zero angles set. Press START to continue.");
        }

        if (_goal_button.was_pressed() && camera)
        {
            camera->toggle_own_goal();
            Serial.print("Own goal: ");
            Serial.println(camera->own_goal() == GoalColor::YELLOW ? "YELLOW" : "BLUE");
        }

        return _running;
    }

private:
    Button _start_stop;
    Button _gyro_button;
    Button _goal_button;
    bool _running = false;
};
