from typing import NoReturn

from image import Image
import sensor

from OpenMV.CV import DetectingBlob, ObjectDetector
from OpenMV.containers import CameraConfig, ObjectData, RobotAttacker
from OpenMV.maths import DistanceCalculator


class DistanceTest:
    def main(self) -> NoReturn:
        robot_type = RobotAttacker()    # CHANGE THIS FOR EACH ROBOT
        cam_cfg = CameraConfig(robot_type)
        dist_calc = DistanceCalculator()
        obj_detector = ObjectDetector(cam_cfg, dist_calc, DetectingBlob())

        while True:
            img: Image = sensor.snapshot()

            obj_detector.set_strategy(DetectingBlob())
            data: ObjectData = obj_detector.find_object(
                img, cam_cfg.blue_threshold(), robot_type)

            print(data.raw_distance)


def main() -> NoReturn:
    test = DistanceTest()
    test.main()


if __name__ == "__main__":
    main()
