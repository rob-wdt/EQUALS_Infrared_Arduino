import math

from OpenMV.containers import RobotType


class Point:
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

    x: float
    y: float


class DistanceCalculator:
    def euclidean_distance(self, a: int | float, b: int | float) -> float:
        return math.sqrt(a**2 + b**2)

    def linearize(self, robot_type: RobotType, pixels: int) -> float:
        res: tuple[float, float] = (0, 0)
        pixels_abs: int = abs(pixels)

        for i in range(len(robot_type.px_to_cm_table())):
            if pixels_abs >= robot_type.px_to_cm_table()[i][0] and pixels_abs <= robot_type.px_to_cm_table()[i + 1][0]:
                res = robot_type.px_to_cm_table()[i]
                break

        return res[1]
