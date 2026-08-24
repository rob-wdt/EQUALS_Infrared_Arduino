import OpenMV.maths as maths
import sensor

#############################################################################################################################
# ------------------------------------------------ GENERAL ---------------------------------------------------------------- #
UART_NUM: int = 3
UART_BAUDRATE: int = 230400

#############################################################################################################################
# ----------------------------------------------- ATTACKER ---------------------------------------------------------------- #

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

PX_TO_CM_ATTACKER = (
    # PX CM
    (0, 0),
    (0, 0),
    (0, 0),
    (0, 0))


#############################################################################################################################
# ----------------------------------------------- DEFENDER ---------------------------------------------------------------- #

YELLOW_THRESHOLD_DEFENDER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)
BLUE_THRESHOLD_DEFENDER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)
RED_THRESHOLD_DEFENDER: tuple[list[int]] = ([0, 0, 0, 0, 0, 0],)

WHITE_DEFENDER = (0, 0, 0)

FRAMESIZE_DEFENDER: int = sensor.QVGA

FRAME_H_DEFENDER: int = 0
FRAME_W_DEFENDER: int = 0

if FRAMESIZE_DEFENDER == sensor.QVGA:
    FRAME_H_DEFENDER = 0
    FRAME_W_DEFENDER = 0

CENTER_DEFENDER = maths.Point(FRAME_W_DEFENDER // 2, FRAME_H_DEFENDER // 2)

MASK_RADIUS_DEFENDER: int = 0

ROBOT_RADIUS_DEFENDER: int = 0

GAIN_DEFENDER = 0

EXPOSURE_DEFENDER = 0

PX_TO_CM_DEFENDER = (
    # PX CM
    (0, 0),
    (0, 0),
    (0, 0),
    (0, 0))
