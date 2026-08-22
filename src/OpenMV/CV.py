from abc import abstractmethod
import math
from typing import Any, List

from OpenMV import maths
from OpenMV.containers import CameraConfig, ObjectData, RobotType
from OpenMV.maths import DistanceCalculator
from image import blob

import sensor


class DetectingStrategy:
    @abstractmethod
    def get_blobs(self, img, threshold) -> List[blob]:
        pass


class DetectingBlob(DetectingStrategy):
    def get_blobs(self, img: sensor.image.Image, threshold: Any) -> List[blob]:
        blobs: List[blob] = img.find_blobs(threshold)
        blobs = sorted(blobs, key=lambda b: b.area(), reverse=True)
        return blobs


class DetectingRound(DetectingStrategy):
    def get_blobs(self, img: sensor.image.Image, threshold: Any) -> List[blob]:
        blobs: List[blob] = img.find_blobs(threshold)
        blobs = sorted(blobs, key=lambda b: b.roundness(), reverse=True)
        return blobs


class ObjectDetector:
    def __init__(self, cam_config: CameraConfig, dist_calc: DistanceCalculator, init_strategy: DetectingStrategy) -> None:
        self.__cm_cfg = cam_config
        self.__dst_clc = dist_calc
        self.__strategy = init_strategy

    def set_strategy(self, strategy: DetectingStrategy) -> None:
        self.__strategy = strategy

    def find_object(self, img: sensor.image.Image, threshold: Any, robot_type: RobotType) -> ObjectData:
        blobs: List[blob] = self.__strategy.get_blobs(img, threshold)
        if not blobs:
            return ObjectData(maths.Point(0, 0), 0, 0, maths.Point(0, 0), 0, 0, 0)

        max_blob: blob = blobs[0]
        w: float = -(max_blob.cx() - self.__cm_cfg.center().x)
        h: float = -(max_blob.cy() - self.__cm_cfg.center().y)
        distance: float = self.__dst_clc.linearize(
            robot_type, int(self.__dst_clc.euclidean_distance(w, h)))

        angle: int = math.floor(math.degrees(math.atan2(w, h)))

        return ObjectData(maths.Point(max_blob.cx(), max_blob.cy()), angle, distance, maths.Point(max_blob.x(), max_blob.y()), max_blob.w(), max_blob.h(), self.__dst_clc.euclidean_distance(w, h))

    __cm_cfg: CameraConfig
    __dst_clc: DistanceCalculator
    __strategy: DetectingStrategy
