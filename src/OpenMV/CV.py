import math

import maths
import settings
from containers import CameraConfig, ObjectData, RobotType
from perception import DataSender
from image import Image
import sensor


class DistanceCalculator:
    def euclidean_distance(self, a: int | float, b: int | float) -> float:
        return math.sqrt(a**2 + b**2)

    def linearize(self, robot_type: RobotType, pixels: int) -> float:
        res: tuple[float, float] = (0, 0)
        pixels_abs: int = abs(pixels)

        for i in range(len(robot_type.px_to_cm_table())):
            if i < len(robot_type.px_to_cm_table()) - 1:
                if pixels_abs >= robot_type.px_to_cm_table()[i][0] and pixels_abs <= robot_type.px_to_cm_table()[i + 1][0]:
                    res = robot_type.px_to_cm_table()[i]
                    break

            else:
                res = robot_type.px_to_cm_table()[-1]

        return res[1]


class DetectingStrategy:
    def get_blobs(self, img, threshold):
        pass


class DetectingBlob(DetectingStrategy):
    def get_blobs(self, img: sensor.image.Image, threshold):
        blobs = img.find_blobs(threshold)
        blobs = sorted(blobs, key=lambda b: b.area(), reverse=True)
        return blobs


class DetectingRound(DetectingStrategy):
    def get_blobs(self, img: sensor.image.Image, threshold):
        blobs = img.find_blobs(threshold)
        blobs = sorted(blobs, key=lambda b: b.roundness(), reverse=True)
        return blobs


class ObjectDetector:
    def __init__(self, cam_config: CameraConfig, dist_calc: DistanceCalculator, init_strategy: DetectingStrategy) -> None:
        self.__cm_cfg = cam_config
        self.__dst_clc = dist_calc
        self.__strategy = init_strategy

    def set_strategy(self, strategy: DetectingStrategy):
        self.__strategy = strategy
        return self

    def find_object(self, img: sensor.image.Image, threshold, robot_type: RobotType) -> ObjectData:
        blobs = self.__strategy.get_blobs(img, threshold)
        if not blobs:
            return ObjectData(maths.Point(0, 0), 0, 0, maths.Point(0, 0), 0, 0, 0)

        max_blob = blobs[0]
        w: float = -(max_blob.cx() - self.__cm_cfg.center().x)
        h: float = -(max_blob.cy() - self.__cm_cfg.center().y)
        distance: float = self.__dst_clc.linearize(
            robot_type, int(self.__dst_clc.euclidean_distance(w, h)))

        angle: int = math.floor(math.degrees(math.atan2(w, h)))

        return ObjectData(maths.Point(max_blob.cx(), max_blob.cy()), angle, distance, maths.Point(max_blob.x(), max_blob.y()), max_blob.w(), max_blob.h(), self.__dst_clc.euclidean_distance(w, h))

    __cm_cfg: CameraConfig
    __dst_clc: DistanceCalculator
    __strategy: DetectingStrategy


class VisionSystem:
    def __init__(self, robot_type: RobotType) -> None:
        self.__robot_type = robot_type
        self.__cm_cfg = CameraConfig(self.__robot_type)
        self.__dist_calc = DistanceCalculator()
        self.__detector = ObjectDetector(
            self.__cm_cfg, self.__dist_calc, DetectingBlob())
        self.__sender = DataSender(settings.UART_NUM, settings.UART_BAUDRATE)

    def init(self) -> None:
        # Camera initialization

        sensor.reset()

        sensor.set_pixformat(self.__cm_cfg.pixels_format())
        sensor.set_framesize(self.__cm_cfg.framesize())
        sensor.set_auto_gain(True)
        sensor.set_auto_whitebal(True)
        sensor.set_auto_exposure(True)

        sensor.skip_frames(time=1000)

        sensor.set_pixformat(self.__cm_cfg.pixels_format())
        sensor.set_framesize(self.__cm_cfg.framesize())
        sensor.set_auto_gain(False, self.__cm_cfg.gain())
        sensor.set_auto_whitebal(False, self.__cm_cfg.white())
        sensor.set_auto_exposure(False, self.__cm_cfg.exposure())

        sensor.skip_frames(time=1000)

        # UART initialization
        self.__sender.init()

    def capture_frame(self) -> Image:
        img: Image = sensor.snapshot()

        # limit the detecting area
        img.mask_circle(int(self.__cm_cfg.center().x), int(self.__cm_cfg.center(
        ).y), self.__cm_cfg.mask_radius())

        # hide the robot
        img.draw_circle(int(self.__cm_cfg.center().x), int(self.__cm_cfg.center(
        ).y), self.__cm_cfg.robot_radius(), color=(0, 0, 0), fill=True)

        return img

    def process_frame(self, draw_objects_scopes: bool, draw_lines: bool) -> dict[str, ObjectData]:
        img: Image = self.capture_frame()

        blue_goal: ObjectData = self.__detector.set_strategy(DetectingBlob()).find_object(
            img, self.__cm_cfg.blue_threshold(), self.__robot_type)

        yellow_goal: ObjectData = self.__detector.set_strategy(DetectingBlob()).find_object(
            img, self.__cm_cfg.yellow_threshold(), self.__robot_type)

        ball: ObjectData = self.__detector.set_strategy(DetectingRound()).find_object(
            img, self.__cm_cfg.red_threshold(), self.__robot_type)

        if draw_objects_scopes:
            self.__draw_debug_info(
                img, blue_goal, yellow_goal, ball, draw_lines)

        return {"blue goal": blue_goal, "yellow goal": yellow_goal, "ball": ball}

    def draw_debug_info(self, img: sensor.image.Image, blue_goal: ObjectData, yellow_goal: ObjectData, ball: ObjectData, draw_lines: bool) -> None:
        if blue_goal:
            img.draw_rectangle(int(blue_goal.left_upper_corner.x), int(
                blue_goal.left_upper_corner.y), blue_goal.w, blue_goal.h, color=(0, 0, 0), thickness=5)

            if draw_lines:
                img.draw_line(int(self.__cm_cfg.center().x), int(self.__cm_cfg.center().y), int(
                    blue_goal.center.x), int(blue_goal.center.y), color=(255, 255, 255), thickness=5)

        if yellow_goal:
            img.draw_rectangle(int(yellow_goal.left_upper_corner.x), int(
                yellow_goal.left_upper_corner.y), yellow_goal.w, yellow_goal.h, color=(0, 0, 0), thickness=5)

            if draw_lines:
                img.draw_line(int(self.__cm_cfg.center().x), int(self.__cm_cfg.center().y), int(
                    yellow_goal.center.x), int(yellow_goal.center.y), color=(255, 255, 255), thickness=5)

        if ball:
            img.draw_circle(int(ball.center.x), int(ball.center.y),
                            ball.w, color=(255, 0, 0), thickness=3)

            if draw_lines:
                img.draw_line(int(self.__cm_cfg.center().x), int(self.__cm_cfg.center().y), int(
                    ball.center.x), int(ball.center.y), color=(255, 255, 255), thickness=5)

    def run(self, draw_objects_scopes: bool, draw_lines: bool):
        while True:
            try:
                data: dict[str, ObjectData] = self.process_frame(
                    draw_objects_scopes, draw_lines)

                self.__sender.send((data["blue goal"].angle, int(data["blue goal"].distance), data["yellow goal"].angle,
                                    int(data["yellow goal"].distance), data["ball"].angle, int(data["ball"].distance)))
            except Exception as e:
                print(f"Error: {e}")

    __robot_type: RobotType
    __cm_cfg: CameraConfig
    __dist_calc: DistanceCalculator
    __detector: ObjectDetector
    __sender: DataSender
