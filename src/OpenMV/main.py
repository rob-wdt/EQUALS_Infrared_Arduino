from abc import abstractmethod

import point
import settings
import sensor


class RobotType:
    @abstractmethod
    def __init__(self) -> None:
        pass

    def yellow_threshold(self) -> tuple[list[int]]:
        return self.__yellow_threshold

    def blue_threshold(self) -> tuple[list[int]]:
        return self.__blue_threshold

    def red_threshold(self) -> tuple[list[int]]:
        return self.__red_threshold

    def white(self) -> tuple[int, int, int]:
        return self.__white

    def center(self) -> point.Point:
        return self.__center

    def gain(self) -> int:
        return self.__gain

    def exposure(self) -> int:
        return self.__exposure

    def px_to_cm_table(self) -> tuple[tuple[float, float], ...]:
        return self.__pixels_to_cm_table

    __yellow_threshold: tuple[list[int]]
    __blue_threshold: tuple[list[int]]
    __red_threshold: tuple[list[int]]
    __white: tuple[int, int, int]
    __center: point.Point
    __gain: int
    __exposure: int

    __pixels_to_cm_table: tuple[tuple[float, float], ...]


class RobotAttacker(RobotType):
    def __init__(self) -> None:
        super().__init__()

        self.__yellow_threshold = settings.YELLOW_THRESHOLD_ATTACKER
        self.__blue_threshold = settings.BLUE_THRESHOLD_ATTACKER
        self.__red_threshold = settings.RED_THRESHOLD_ATTACKER
        self.__white = settings.WHITE_ATTACKER
        self.__center = settings.CENTER_ATTACKER
        self.__gain = settings.GAIN_ATTACKER
        self.__exposure = settings.EXPOSURE_ATTACKER
        self.__pixels_to_cm_table = settings.PX_TO_CM_ATTACKER


class RobotDefender(RobotType):
    def __init__(self) -> None:
        super().__init__()

        self.__yellow_threshold = settings.YELLOW_THRESHOLD_DEFENDER
        self.__blue_threshold = settings.BLUE_THRESHOLD_DEFENDER
        self.__red_threshold = settings.RED_THRESHOLD_DEFENDER
        self.__white = settings.WHITE_DEFENDER
        self.__center = settings.CENTER_DEFENDER
        self.__gain = settings.GAIN_DEFENDER
        self.__exposure = settings.EXPOSURE_DEFENDER
        self.__pixels_to_cm_table = settings.PX_TO_CM_DEFENDER


class CameraConfig:
    def __init__(self, robot_type: RobotType) -> None:
        self.__yellow_threshold = robot_type.yellow_threshold()
        self.__blue_threshold = robot_type.blue_threshold()
        self.__red_threshold = robot_type.red_threshold()
        self.__white = robot_type.white()
        self.__center = robot_type.center()
        self.__gain = robot_type.gain()
        self.__exposure = robot_type.exposure()
        self.__pixels_to_cm_table = robot_type.px_to_cm_table()

    def yellow_threshold(self) -> tuple[list[int]]:
        return self.__yellow_threshold

    def blue_threshold(self) -> tuple[list[int]]:
        return self.__blue_threshold

    def red_threshold(self) -> tuple[list[int]]:
        return self.__red_threshold

    def white(self) -> tuple[int, int, int]:
        return self.__white

    def center(self) -> point.Point:
        return self.__center

    def gain(self) -> int:
        return self.__gain

    def exposure(self) -> int:
        return self.__exposure

    def px_to_cm_table(self) -> tuple[tuple[float, float], ...]:
        return self.__pixels_to_cm_table

    __yellow_threshold: tuple[list[int]]
    __blue_threshold: tuple[list[int]]
    __red_threshold: tuple[list[int]]
    __white: tuple[int, int, int]
    __center: point.Point
    __gain: int
    __exposure: int

    __pixels_to_cm_table: tuple[tuple[float, float], ...]


def main() -> None:
    pass


main()
