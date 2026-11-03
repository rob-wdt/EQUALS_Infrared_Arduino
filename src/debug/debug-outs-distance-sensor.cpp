#pragma once

#include <Arduino.h>

#include "control/outs-handler-distance-sensor/outs-handler-distance-sensor.h"
#include "control/movement-controller/movement-controller.h"
#include "management/config/robot-config.h"
#include "control/heading-controller/heading-controller.h"
#include "core/sensors/button/button.h"
#include "core/sensors/BNO055/BNO055.h"
#include "core/led/led.h"
#include "core/sensors/distance-sensor/distance-sensor.h"

#define SPEED 0

class OutsDistanceSensorTest
{
public:
    OutsDistanceSensorTest(DistanceSensor ds, OutsHandlerDistanceSensor oh, MovementController mc, HeadingController align, BNO055 gyro, Button btn, Led led) : _ds(ds), _oh(oh), _mc(mc), _align(align), _gyro(gyro), _button(btn), _led(led) {}
    void setup()
    {
        _mc.init();

        _gyro.init();
        _button.init();
        _led.init();
        Serial.begin(115200);

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to calibrate the gyroscope.");
        _button.wait_click();

        while (!_gyro.calibrated())
            ;
        _led.on();

        Serial.println("Gyroscope calibrated");
        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to set the zero angles.");
        _button.wait_click();
        _gyro.set_zero_angles();

        Serial.print("Press button #");
        Serial.print(_button.pin());
        Serial.println(" to start.");
        _button.wait_click();
    }
    void loop(int lin_speed)
    {
        _gyro.read();
        _ds.read();
        _oh.handle(_ds.data(), 0);

        float angular_speed = _align.calculate(0, _gyro.data());

        if (_oh.in_out())
            _mc.set_velocity(lin_speed, 0, angular_speed);
        else
            _mc.set_velocity(lin_speed, 180, angular_speed);
    }

private:
    DistanceSensor _ds;
    OutsHandlerDistanceSensor _oh;
    MovementController _mc;
    HeadingController _align;
    BNO055 _gyro;
    Button _button;
    Led _led;
};

RobotWhite robot_type;
RobotConfig config(robot_type);

DistanceSensor ds(config.pins().distane_sensor);
OutsHandlerDistanceSensor oh;

MovementController movement_controller(
    {Motor(config.pins().m1_1, config.pins().m1_2, config.motors_config().directions.m1_dir),
     Motor(config.pins().m2_1, config.pins().m2_2, config.motors_config().directions.m2_dir),
     Motor(config.pins().m3_1, config.pins().m3_2, config.motors_config().directions.m3_dir),
     Motor(config.pins().m4_1, config.pins().m4_2, config.motors_config().directions.m4_dir)},
    config.motors_config().loc_angles);
HeadingController align(new PIDControl(config.controls_config().pid));

BNO055 gyro(config.gyro_address());
Button button(config.pins().btn1);
Led led(config.pins().led);

OutsDistanceSensorTest test(ds, oh, movement_controller, align, gyro, button, led);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop(SPEED);
}
