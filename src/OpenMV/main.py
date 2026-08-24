import CV
from OpenMV.containers import RobotAttacker, RobotDefender


def main() -> None:
    robot_type = RobotAttacker()
    vision = CV.VisionSystem(robot_type)

    vision.run(draw_objects_scopes=True, draw_lines=True)


if __name__ == "__main__":
    main()
