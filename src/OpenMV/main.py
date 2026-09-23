import CV
from containers import RobotAttacker, RobotDefender


def main() -> None:
    robot_type = RobotDefender()
    vision = CV.VisionSystem(robot_type)

    vision.init()

    vision.run(draw_objects_scopes=True, draw_lines=True)


if __name__ == "__main__":
    main()
