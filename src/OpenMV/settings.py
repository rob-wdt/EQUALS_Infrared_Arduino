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

YELLOW_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 0, 0, 0, 0, 0)]
BLUE_THRESHOLD_DEFENDER: tuple[list[int]] = [(57, 100, -37, -18, -19, 9)]
RED_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 0, 0, 0, 0, 0)]

WHITE_DEFENDER = (62, 60, 62)

FRAMESIZE_DEFENDER: int = sensor.QVGA

FRAME_H_DEFENDER: int
FRAME_W_DEFENDER: int

if FRAMESIZE_DEFENDER == sensor.QVGA:
    FRAME_H_DEFENDER = 240
    FRAME_W_DEFENDER = 320

CENTER_DEFENDER = maths.Point(FRAME_W_DEFENDER // 2 + 15, FRAME_H_DEFENDER // 2 - 6)

MASK_RADIUS_DEFENDER: int = 100

ROBOT_RADIUS_DEFENDER: int = 20

GAIN_DEFENDER = 50

EXPOSURE_DEFENDER = 100_000

PIXELS_FORMAT_DEFENDER = sensor.RGB565

PX_TO_CM_DEFENDER = (
    # PX CM
    (30, 10),
    (30, 15),
    (18, 20),
    (23, 25),
    (27, 30),
    (33, 35),
    (37, 40),
    (41, 45),
    (45, 50),
    (49, 55),
    (51, 60),
    (54, 65),
    (57, 70),
    (60, 75),
    (63, 80),
    (65, 85),
    (67, 90),
    (69, 95),
    (71, 100),
    (73, 105),
    (75, 110),
    (77, 115),
    (78, 120),
    (79, 125),
    (80, 130),
    (81, 135),
    (81, 140),
    (82, 145),
    (83, 150),
    (84, 155),
    (85, 160),
    (85, 165),
    (85, 170),
    (86, 175),
    (86, 180),
    (87, 185),
    (87, 190),
    (87, 195),
    (89, 200),
    (89, 205),
    (89, 210),
    (89, 215),
    (90, 220),
    (90, 225))
