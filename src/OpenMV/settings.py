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

YELLOW_THRESHOLD_DEFENDER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)
BLUE_THRESHOLD_DEFENDER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)
RED_THRESHOLD_DEFENDER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)

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

EXPOSURE_DEFENDER = 10_000

PIXELS_FORMAT_DEFENDER = sensor.RGB565

PX_TO_CM_DEFENDER = (
    # PX CM
    (0, 0),
    (0, 0),
    (0, 0),
    (0, 0))
