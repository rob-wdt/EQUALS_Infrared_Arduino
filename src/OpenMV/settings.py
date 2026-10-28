import maths
import sensor

########################################################################################################################
# ------------------------------------------------ GENERAL ----------------------------------------------------------- #
UART_NUM: int = 3
UART_BAUDRATE: int = 230400

########################################################################################################################
# ----------------------------------------------- ATTACKER ----------------------------------------------------------- #

FRAMESIZE_ATTACKER: int = sensor.QVGA

if FRAMESIZE_ATTACKER == sensor.QVGA:
    FRAME_H_ATTACKER = 240
    FRAME_W_ATTACKER = 320

YELLOW_THRESHOLD_ATTACKER: tuple[list[int]] = [(85, 100, -13, 12, 16, 37)]
BLUE_THRESHOLD_ATTACKER: tuple[list[int]] = [(58, 72, -17, 2, -27, -7)]
RED_THRESHOLD_ATTACKER: tuple[list[int]] = [(0, 100, 19, 58, -41, 127)]

WHITE_ATTACKER = (64, 60, 64)

CENTER_ATTACKER = maths.Point(FRAME_W_ATTACKER // 2 + 10, FRAME_H_ATTACKER // 2 - 12)
MASK_RADIUS_ATTACKER: int = 120
ROBOT_RADIUS_ATTACKER: int = 15
GAIN_ATTACKER = 24
EXPOSURE_ATTACKER = 21450

PIXELS_FORMAT_ATTACKER = sensor.RGB565

PX_TO_CM_ATTACKER = (
    # PX CM
    (11, 10),
    (17, 15),
    (24, 20),
    (30, 25),
    (35, 30),
    (41, 35),
    (45, 40),
    (50, 45),
    (56, 50),
    (60, 55),
    (65, 60),
    (70, 65),
    (74, 70),
    (77, 75),
    (81, 80),
    (84, 85),
    (86, 90),
    (88, 95),
    (91, 100),
    (92, 105),
    (93, 110),
    (94, 115),
    (95, 120),
    (97, 125),
    (98, 130),
    (99, 135),
    (101, 140),
    (102, 145),
    (103, 150),
    (103, 155),
    (104, 160),
    (104, 165),
    (120, 210))


########################################################################################################################
# ----------------------------------------------- DEFENDER ----------------------------------------------------------- #

FRAMESIZE_DEFENDER: int = sensor.QVGA

if FRAMESIZE_DEFENDER == sensor.QVGA:
    FRAME_H_DEFENDER = 240
    FRAME_W_DEFENDER = 320

YELLOW_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, -20, 127, 29, 127)]
BLUE_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, -128, 16, -128, -10)]
RED_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, 12, 127, -5, 127)]

WHITE_DEFENDER = (64, 60, 64)

CENTER_DEFENDER = maths.Point(FRAME_W_DEFENDER // 2 - 8, FRAME_H_DEFENDER // 2 - 14)
MASK_RADIUS_DEFENDER: int = 120
ROBOT_RADIUS_DEFENDER: int = 13
GAIN_DEFENDER = 24
EXPOSURE_DEFENDER = 21450

PIXELS_FORMAT_DEFENDER = sensor.RGB565

PX_TO_CM_DEFENDER = (
    # PX CM
    (11, 10),
    (17, 15),
    (24, 20),
    (30, 25),
    (35, 30),
    (41, 35),
    (46, 40),
    (50, 45),
    (55, 50),
    (57, 55),
    (61, 60),
    (65, 65),
    (68, 70),
    (72, 75),
    (74, 80),
    (76, 85),
    (79, 90),
    (81, 95),
    (82, 100),
    (83, 105),
    (84, 110),
    (86, 115),
    (87, 120),
    (89, 125),
    (92, 130),
    (93, 135),
    (94, 140),
    (96, 145),
    (98, 150),
    (99, 155),
    (100, 160),
    (101, 165),
    (103, 210))
