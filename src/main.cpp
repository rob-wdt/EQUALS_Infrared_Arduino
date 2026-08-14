#include "config.h"
#include "core/sensors/phototransistor/phototransistor.h"
#include "core/sensors/button/button.h"
#include "core/motor/motor.h"
#include "core/sensors/BNO055/BNO055.h"
#include "utils/control/PDC-control/PDC-control.h"
#include "utils/control/PID-control/PID-control.h"
#include "control/movement-controller/movement-controller.h"
#include "control/ball-detector/ball-detector.h"

#if DEBUG_MODE == DEBUG_MODE_NONE
void setup()
{
}

void loop()
{
}

#elif DEBUG_MODE == DEBUG_MODE_PHOTOTRANSISTOR
Phototransistor pt(Pin::PIN_PHOTOTRANSISTOR);

void setup()
{
    pt.init();
    Serial.begin(115200);
}

void loop()
{
    pt.read();
    Serial.print("Phototr. data: ");
    Serial.println(pt.data());
}

#elif DEBUG_MODE == DEBUG_MODE_BUTTON
Button button(Pin::PIN_BTN_1);

void setup()
{
    button.init();
}

void loop()
{
    button.read();
    Serial.print("Button data: ");
    Serial.println(button.is_pressed());
}

#elif DEBUG_MODE == DEBUG_MODE_MOTORS
Motor m1(Pin::PIN_M1_1, Pin::PIN_M1_2, MotorDirection::forward);
Motor m2(Pin::PIN_M2_1, Pin::PIN_M2_2, MotorDirection::forward);
Motor m3(Pin::PIN_M3_1, Pin::PIN_M3_2, MotorDirection::forward);
Motor m4(Pin::PIN_M4_1, Pin::PIN_M4_2, MotorDirection::forward);

void setup()
{
    m1.init();
    m2.init();
    m3.init();
    m4.init();
}

void loop()
{
    m1.set_speed(SPEED);
    m2.set_speed(SPEED);
    m3.set_speed(SPEED);
    m4.set_speed(SPEED);

    m1.run();
    m2.run();
    m3.run();
    m4.run();
}

#elif DEBUG_MODE == DEBUG_MODE_GYROSCOPE
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

void setup()
{
    gyro.init();
    Serial.begin(115200);

    Serial.print("Press button #");
    Serial.print(Pin::PIN_BTN_1);
    Serial.println(" to calibrate the gyroscope.");
    while (!button.is_pressed())
        ;

    while (!gyro.calibrated())
        ;

    Serial.println("Gyroscope calibrated");
    Serial.println("Press button #");
    Serial.print(Pin::PIN_BTN_1);
    Serial.println(" to set the zero angles.");
    while (!button.is_pressed())
        ;

    gyro.set_zero_angles();
}

void loop()
{
    gyro.read();
    gyroData data = gyro.data();

    Serial.print("Gyro data: ");
    Serial.print("Roll: ");
    Serial.print(data.roll);
    Serial.print(", Pitch: ");
    Serial.print(data.pitch);
    Serial.print(", Yaw: ");
    Serial.println(data.yaw);
}

#elif DEBUG_MODE == DEBUG_MODE_PID
PIDControl pid(control_gyro_align_pid);
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

void setup()
{
    gyro.init();
    Serial.begin(115200);

    Serial.print("Press button #");
    Serial.print(Pin::PIN_BTN_1);
    Serial.println(" to calibrate the gyroscope.");
    while (!button.is_pressed())
        ;

    while (!gyro.calibrated())
        ;

    Serial.println("Gyroscope calibrated");
    Serial.println("Press button #");
    Serial.print(Pin::PIN_BTN_1);
    Serial.println(" to set the zero angles.");
    while (!button.is_pressed())
        ;

    gyro.set_zero_angles();
}

void loop()
{
    gyro.read();
    gyroData data = gyro.data();

    double pid_output = pid.calculate(data.yaw);

    Serial.println(pid_output);
}

#elif DEBUG_MODE == DEBUG_MODE_PDC
PDCControl pdc(control_gyro_align_pdc);
BNO055 gyro(GYROSCOPE_ADDRESS);
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

void setup()
{
    gyro.init();
    Serial.begin(115200);

    Serial.print("Press button #");
    Serial.print(Pin::PIN_BTN_1);
    Serial.println(" to calibrate the gyroscope.");
    while (!button.is_pressed())
        ;

    while (!gyro.calibrated())
        ;

    Serial.println("Gyroscope calibrated");
    Serial.println("Press button #");
    Serial.print(Pin::PIN_BTN_1);
    Serial.println(" to set the zero angles.");
    while (!button.is_pressed())
        ;

    gyro.set_zero_angles();
}

void loop()
{
    gyro.read();
    gyroData data = gyro.data();

    double pdc_output = pdc.calculate(data.yaw);

    Serial.println(pdc_output);
}

#elif DEBUG_MODE == DEBUG_MODE_MOVEMENT_CONTROLLER
MovementController movement_controller(
    {Motor(Pin::PIN_M1_1, Pin::PIN_M1_2, MotorDirection::forward),
     Motor(Pin::PIN_M2_1, Pin::PIN_M2_2, MotorDirection::forward),
     Motor(Pin::PIN_M3_1, Pin::PIN_M3_2, MotorDirection::forward),
     Motor(Pin::PIN_M4_1, Pin::PIN_M4_2, MotorDirection::forward)},
    {MotorLocationAngle::M1_ANGLE, MotorLocationAngle::M2_ANGLE, MotorLocationAngle::M3_ANGLE, MotorLocationAngle::M4_ANGLE},
    BNO055(GYROSCOPE_ADDRESS),
    new PDCControl(control_gyro_align_pdc));

void setup()
{
    movement_controller.init();
    Serial.begin(115200);
}

void loop()
{
    movement_controller.read_gyro();
    movement_controller.set_velocity(SPEED, 0, 0);
    movement_controller.move();
}

#elif DEBUG_MODE == DEBUG_MODE_BALL_DETECTOR
BallDetector ball_detector(Phototransistor(Pin::PIN_PHOTOTRANSISTOR), 50);

void setup()
{
    ball_detector.init();
    Serial.begin(115200);
}

void loop()
{
    ball_detector.read();
    Serial.print("Ball detected: ");
    Serial.println(ball_detector.ball_detected());
}

#endif
