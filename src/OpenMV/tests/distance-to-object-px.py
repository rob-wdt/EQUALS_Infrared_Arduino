from image import Image

from CV import ObjectDetector
from CV import DetectingBlob
from CV import VisionSystem
import containers
from CV import DistanceCalculator

import maths


class DistancePXTest:
    def main(self):
        robot_type = containers.RobotDefender()   # CHANGE THIS FOR EACH ROBOT
        cam_cfg = containers.CameraConfig(robot_type)
        vs_system = VisionSystem(robot_type)
        dist_calc = DistanceCalculator()
        obj_detector = ObjectDetector(cam_cfg, dist_calc, DetectingBlob())

        vs_system.init()

        while True:
            img: Image = vs_system.capture_frame()

            obj_detector.set_strategy(DetectingBlob())
            data: containers.ObjectData = obj_detector.find_object(
                img, cam_cfg.blue_threshold(), robot_type)

            vs_system.draw_debug_info(img, data, containers.ObjectData(maths.Point(0, 0), 0, 0, maths.Point(0, 0), 0, 0, 0), containers.ObjectData(maths.Point(0, 0), 0, 0, maths.Point(0, 0), 0, 0, 0), True)

            print(data.raw_distance)


def main():
    test = DistancePXTest()
    test.main()


if __name__ == "__main__":
    main()
