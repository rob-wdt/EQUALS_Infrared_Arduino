import maths
import sensor

########################################################################################################################
# ------------------------------------------------ GENERAL ----------------------------------------------------------- #
UART_NUM: int = 3
UART_BAUDRATE: int = 230400

########################################################################################################################
# ----------------------------------------------- ATTACKER ----------------------------------------------------------- #

YELLOW_THRESHOLD_ATTACKER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)
BLUE_THRESHOLD_ATTACKER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)
RED_THRESHOLD_ATTACKER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)

WHITE_ATTACKER = (0, 0, 0)

FRAMESIZE_ATTACKER: int = sensor.QVGA

FRAME_H_ATTACKER: int = 0
FRAME_W_ATTACKER: int = 0

if FRAMESIZE_ATTACKER == sensor.QVGA:
    FRAME_H_ATTACKER = 0
    FRAME_W_ATTACKER = 0

CENTER_ATTACKER = maths.Point(FRAME_W_ATTACKER // 2, FRAME_H_ATTACKER // 2)

MASK_RADIUS_ATTACKER: int = 0

ROBOT_RADIUS_ATTACKER: int = 0

GAIN_ATTACKER = 0

EXPOSURE_ATTACKER = 0

PIXELS_FORMAT_ATTACKER = sensor.RGB565

PX_TO_CM_ATTACKER = (
    # PX CM
    (0, 0),
    (0, 0),
    (0, 0),
    (0, 0))


########################################################################################################################
# ----------------------------------------------- DEFENDER ----------------------------------------------------------- #

YELLOW_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, -128, 127, 41, 127)]
BLUE_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, -128, -12, -128, 7)]
RED_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, 0, 127, 20, 40)]

WHITE_DEFENDER = (63, 59, 61)

FRAMESIZE_DEFENDER: int = sensor.QVGA

if FRAMESIZE_DEFENDER == sensor.QVGA:
    FRAME_H_DEFENDER = 240
    FRAME_W_DEFENDER = 320

CENTER_DEFENDER = maths.Point(FRAME_W_DEFENDER // 2 - 3, FRAME_H_DEFENDER // 2 - 13)

MASK_RADIUS_DEFENDER: int = 120

ROBOT_RADIUS_DEFENDER: int = 20

GAIN_DEFENDER = 23

EXPOSURE_DEFENDER = 100_000

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
    (81, 100),
    (83, 105),
    (84, 110),
    (86, 115),
    (87, 120),
    (87, 125),
    (92, 130),
    (93, 135),
    (94, 140),
    (96, 145),
    (98, 150),
    (99, 155),
    (100, 160),
    (100, 165),
    (103, 210))
