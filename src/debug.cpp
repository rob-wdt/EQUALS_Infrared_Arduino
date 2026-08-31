#include "config.h"
#include "core/sensors/phototransistor/phototransistor.h"
#include "core/sensors/button/button.h"
#include "core/motor/motor.h"
#include "core/sensors/BNO055/BNO055.h"
#include "utils/control/PDC-control/PDC-control.h"
#include "utils/control/PID-control/PID-control.h"
#include "control/movement-controller/movement-controller.h"
#include "control/ball-detector/ball-detector.h"
#include "core/UART/UART.h"
#include "control/coordinates-calculator/coordinates-calculator.h"
#include "control/outs-handler/outs-handler.h"

#if DEBUG_MODE == DEBUG_MODE_PHOTOTRANSISTOR
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
Button button(Pin::PIN_BTN_3);

void setup()
{
    button.init();
    Serial.begin(115200);
}

void loop()
{
    button.read();
    Serial.print("Button data: ");
    Serial.println(button.is_pressed());
}

#elif DEBUG_MODE == DEBUG_MODE_MOTORS
Motor m1(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir);
Motor m2(Pin::PIN_M2_1, Pin::PIN_M2_2, motors_directions.m2_dir);
Motor m3(Pin::PIN_M3_1, Pin::PIN_M3_2, motors_directions.m3_dir);
Motor m4(Pin::PIN_M4_1, Pin::PIN_M4_2, motors_directions.m4_dir);

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
    {Motor(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir),
     Motor(Pin::PIN_M2_1, Pin::PIN_M2_2, motors_directions.m2_dir),
     Motor(Pin::PIN_M3_1, Pin::PIN_M3_2, motors_directions.m3_dir),
     Motor(Pin::PIN_M4_1, Pin::PIN_M4_2, motors_directions.m4_dir)},
    motors_loc_angles,
    new PDCControl(control_gyro_align_pdc));

BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);

#define MOVE_ANGLE 0
#define ALIGN_ANGLE 0

void setup()
{
    gyro.init();
    movement_controller.init();
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

    movement_controller.set_velocity(SPEED, MOVE_ANGLE, ALIGN_ANGLE, data);
    movement_controller.move();
}

#elif DEBUG_MODE == DEBUG_MODE_BALL_DETECTOR

#define THRESHOLD 1

BallDetector ball_detector(Phototransistor(Pin::PIN_PHOTOTRANSISTOR), THRESHOLD);

void setup()
{
    ball_detector.init();
    Serial.begin(115200);
}

void loop()
{
    ball_detector.read();
    Serial.print("Ball detected: ");
    Serial.print(ball_detector.ball_detected());
    Serial.print("\tPhtstr data: ");
    Serial.print(ball_detector.phototransistor_data());
    Serial.print("\tThreshold: ");
    Serial.println(THRESHOLD);
}

#elif DEBUG_MODE == DEBUG_MODE_COORDS
CoordinatesCalculator coords;
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);
UART uart(UART_NUM::UART_1, 230400);
CameraDataParser cam_parser(uart);

void setup()
{
    gyro.init();
    cam_parser.init();
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
    cam_parser.parse();
    coords.calculate_coordinates(gyro.data(), cam_parser.data().own_goal_angle, cam_parser.data().own_goal_distance);

    Serial.print("Current robot coords:\tx: ");
    Serial.print(coords.coordinates().x);
    Serial.print(" y: ");
    Serial.println(coords.coordinates().y);
}

#elif DEBUG_MODE == DEBUG_MODE_CAM_PERCEPTION
#define UART_BUFFER_SIZE 5

UART uart(UARTNum::UART_3, 230400, UART_BUFFER_SIZE, UART_BUFFER_STARTER);

void setup()
{
    Serial.begin(115200);
    uart.init();
}

void loop()
{
    uart.receive();
    auto data = uart.recieved_data();
    // Serial.print("Received data:\t");

    // for (int i = 0; i < uart.buffer_size(); i++)
    // {
    //     Serial.print(data[i]);
    //     Serial.print("\t");
    // }
    // Serial.println();
}

#elif DEBUG_MODE == DEBUG_MODE_CAM_DATA_PARSER
UART uart(UART_NUM::UART_1, 230400);
CameraDataParser cam_data_parser(uart);

void setup()
{
    cam_data_parser.init();
    Serial.begin(115200);
}

void loop()
{
    cam_data_parser.parse();
    Serial.print("Parsed data:\tBall angle: ");
    Serial.print(cam_data_parser.data().ball_angle);
    Serial.print(" Ball distance: ");
    Serial.print(cam_data_parser.data().ball_distance);
    Serial.print(" Own goal angle: ");
    Serial.print(cam_data_parser.data().own_goal_angle);
    Serial.print(" Own goal distance: ");
    Serial.print(cam_data_parser.data().own_goal_distance);
    Serial.print(" Opp goal angle: ");
    Serial.print(cam_data_parser.data().own_goal_angle);
    Serial.print(" Opp goal distance: ");
    Serial.print(cam_data_parser.data().own_goal_distance);
}

#elif DEBUG_MODE == DEBUG_MODE_OUTS_COMMON
OutsHandler outs;
CoordinatesCalculator coords;
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);
UART uart(UART_NUM::UART_1, 230400);
CameraDataParser cam_parser(uart);

void setup()
{
    gyro.init();
    cam_parser.init();
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
    cam_parser.parse();
    coords.calculate_coordinates(gyro.data(), cam_parser.data().own_goal_angle, cam_parser.data().own_goal_distance);

    outs.handle(coords.coordinates(), OUTS_BOUNDS);

    String out_type;
    bool in_out;

    switch (outs.out_type())
    {
    case OutType::LEFT:
        out_type = "LEFT";
        in_out = true;
        break;
    case OutType::FRONT_GOAL:
        out_type = "FRONT";
        in_out = true;
        break;
    case OutType::NO:
        out_type = "NO";
        in_out = false;
        break;
    case OutType::REAR_GOAL:
        out_type = "REAR";
        in_out = true;
        break;
    case OutType::RIGHT:
        out_type = "RIGHT";
        in_out = true;
        break;
    }

    Serial.print("In out: ");
    Serial.print(in_out);
    Serial.print("\tOut type: ");
    Serial.println(out_type);
}

#elif DEBUG_MODE == DEBUG_MODE_OUTS_MOVEMENT
OutsHandler outs;
MovementController movement_controller(
    {Motor(Pin::PIN_M1_1, Pin::PIN_M1_2, motors_directions.m1_dir),
     Motor(Pin::PIN_M2_1, Pin::PIN_M2_2, motors_directions.m2_dir),
     Motor(Pin::PIN_M3_1, Pin::PIN_M3_2, motors_directions.m3_dir),
     Motor(Pin::PIN_M4_1, Pin::PIN_M4_2, motors_directions.m4_dir)},
    motors_loc_angles,
    new PDCControl(control_gyro_align_pdc));
CoordinatesCalculator coords;
BNO055 gyro(GYROSCOPE_ADDRESS);
Button button(Pin::PIN_BTN_1);
UART uart(UART_NUM::UART_1, 230400);
CameraDataParser cam_parser(uart);

void setup()
{
    gyro.init();
    cam_parser.init();
    movement_controller.init();
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

int move_angle = 0;

void loop()
{
    gyro.read();
    cam_parser.parse();
    coords.calculate_coordinates(gyro.data(), cam_parser.data().own_goal_angle, cam_parser.data().own_goal_distance);

    outs.handle(coords.coordinates(), OUTS_BOUNDS);

    movement_controller.set_velocity(SPEED, move_angle, 0, gyro.data());
    movement_controller.move();

    String out_type;
    bool in_out;

    switch (outs.out_type())
    {
    case OutType::LEFT:
        out_type = "LEFT";
        in_out = true;
        move_angle = 90;
        break;
    case OutType::FRONT_GOAL:
        out_type = "FRONT";
        in_out = true;
        move_angle = 180;
        break;
    case OutType::NO:
        out_type = "NO";
        in_out = false;
        break;
    case OutType::REAR_GOAL:
        out_type = "REAR";
        in_out = true;
        move_angle = 0;
        break;
    case OutType::RIGHT:
        out_type = "RIGHT";
        in_out = true;
        move_angle = -90;
        break;
    }

    Serial.print("In out: ");
    Serial.print(in_out);
    Serial.print("\tOut type: ");
    Serial.println(out_type);
}

#endif
