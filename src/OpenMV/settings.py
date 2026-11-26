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

    OBJECT_DETECTING_PIX_THRESHOLD_ATTACKER = 9
    OBJECT_DETECTING_AREA_THRESHOLD_ATTACKER = 9

YELLOW_THRESHOLD_ATTACKER: tuple[list[int]] = [(71, 84, -3, 14, 26, 127)]
BLUE_THRESHOLD_ATTACKER: tuple[list[int]] = [(38, 52, -17, 3, -23, 0)]
RED_THRESHOLD_ATTACKER: tuple[list[int]] = [(0, 100, 22, 127, -35, 127)]

WHITE_ATTACKER = (64, 60, 64)

CENTER_ATTACKER = maths.Point(FRAME_W_ATTACKER // 2 + 10, FRAME_H_ATTACKER // 2 - 12)
MASK_RADIUS_ATTACKER: int = 115
ROBOT_RADIUS_ATTACKER: int = 10
GAIN_ATTACKER = 20
EXPOSURE_ATTACKER = 21000

PIXELS_FORMAT_ATTACKER = sensor.RGB565

PX_TO_CM_ATTACKER = (
    # PX CM
    (11, 10),
    (12, 11),
    (13, 12),
    (14, 13),
    (15, 14),
    (17, 15),
    (22, 20),
    (29, 25),
    (35, 30),
    (41, 35),
    (46, 40),
    (49, 45),
    (53, 50),
    (56, 55),
    (59, 60),
    (62, 65),
    (66, 70),
    (69, 75),
    (72, 80),
    (73, 85),
    (76, 90),
    (77, 95),
    (79, 100),
    (81, 105),
    (82, 110),
    (83, 115),
    (83, 120),
    (84, 125),
    (85, 130),
    (85, 135),
    (86, 140),
    (87, 145),
    (87, 150),
    (87, 155),
    (88, 160),
    (89, 165),
    (90, 170),
    (90, 175),
    (91, 180),
    (91, 185),
    (92, 190),
    (92, 195),
    (92, 200),
    (92, 205),
    (92, 210))


########################################################################################################################
# ----------------------------------------------- DEFENDER ----------------------------------------------------------- #

FRAMESIZE_DEFENDER: int = sensor.QVGA

if FRAMESIZE_DEFENDER == sensor.QVGA:
    FRAME_H_DEFENDER = 240
    FRAME_W_DEFENDER = 320

    OBJECT_DETECTING_PIX_THRESHOLD_DEFENDER = 9
    OBJECT_DETECTING_AREA_THRESHOLD_DEFENDER = 9

YELLOW_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, -1, 25, 21, 127)]
BLUE_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, -128, -7, -128, -5)]
RED_THRESHOLD_DEFENDER: tuple[list[int]] = [(0, 100, 10, 127, 1, 127)]

WHITE_DEFENDER = (64, 60, 64)

CENTER_DEFENDER = maths.Point(FRAME_W_DEFENDER // 2 - 3, FRAME_H_DEFENDER // 2 - 15)
MASK_RADIUS_DEFENDER: int = 115
ROBOT_RADIUS_DEFENDER: int = 10
GAIN_DEFENDER = 20
EXPOSURE_DEFENDER = 21000

PIXELS_FORMAT_DEFENDER = sensor.RGB565

PX_TO_CM_DEFENDER = (
    # PX CM
    (13, 10),
    (14, 11),
    (15, 12),
    (16, 13),
    (17, 14),
    (19, 15),
    (26, 20),
    (33, 25),
    (39, 30),
    (45, 35),
    (50, 40),
    (54, 45),
    (58, 50),
    (61, 55),
    (65, 60),
    (68, 65),
    (71, 70),
    (74, 75),
    (78, 80),
    (79, 85),
    (82, 90),
    (84, 95),
    (85, 100),
    (89, 105),
    (91, 110),
    (92, 115),
    (93, 120),
    (94, 125),
    (94, 130),
    (95, 135),
    (95, 140),
    (96, 145),
    (97, 150),
    (98, 155),
    (98, 160),
    (99, 165),
    (100, 170),
    (100, 175),
    (101, 180),
    (102, 185),
    (103, 190),
    (104, 195),
    (104, 200),
    (104, 205),
    (105, 210))
