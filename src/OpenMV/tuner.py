# Camera tuner: one self-contained script for setting up the vision parameters.
#
# It doesn't import anything from this project, so it doesn't have to be copied to the camera:
# open it in OpenMV IDE and press Run. Detection works exactly like in CV.py.
#
# How to use:
#   1. Choose ROBOT and MODE below, change the parameters, press Run, look at the frame buffer and the terminal.
#   2. Repeat until it looks right.
#   3. Copy the block printed in the terminal ("copy to settings.py") over the same lines in settings.py.

import math
import time

import sensor

########################################################################################################################
# ------------------------------------------------- WHAT TO DO ------------------------------------------------------- #

# Suffix of the variables in settings.py: "ATTACKER" (violet robot) or "DEFENDER" (white robot)
ROBOT = "DEFENDER"

# "objects"   - all objects as the robot sees them: frames, lines, angles and distances (like main.py)
# "mask"      - only the pixels that fall into the threshold of TARGET are white: tune the thresholds here
# "threshold" - put TARGET into the square at SAMPLE_POS, the terminal shows a threshold that covers its colors
# "center"    - no masking, a cross at CENTER and the MASK_RADIUS / ROBOT_RADIUS circles: put the cross in the
#               middle of the mirror
# "distance"  - pixels from the center to TARGET, for filling PX_TO_CM: put the object at 10, 15, 20 ... cm
# "exposure"  - automatic gain, white balance and exposure: the terminal shows what the camera picked,
#               use it as a starting point for GAIN, WHITE and EXPOSURE
MODE = "distance"

# Object for the "mask", "threshold" and "distance" modes: "yellow", "blue" or "ball"
TARGET = "yellow"

# "threshold" mode: center of the sampling square (image pixels) and its size
SAMPLE_POS = (137, 107)
SAMPLE_SIZE = 3

# How often the terminal is updated, ms
PRINT_EVERY_MS = 300

########################################################################################################################
# ------------------------------------- PARAMETERS (the same as in settings.py) -------------------------------------- #

# LAB thresholds: (L min, L max, A min, A max, B min, B max)
YELLOW_THRESHOLD = [(0, 100, -20, 127, 29, 127)]
BLUE_THRESHOLD = [(0, 100, -128, 16, -128, -10)]
RED_THRESHOLD = [(0, 100, 12, 127, -5, 127)]

WHITE = (64, 60, 64)  # rgb_gain_db
GAIN = 24
EXPOSURE = 21_450

# Shift of the mirror center from the middle of the frame, pixels: CENTER = (W // 2 + x, H // 2 + y)
CENTER_OFFSET = (-8, -14)

MASK_RADIUS = 120
ROBOT_RADIUS = 13

PX_TO_CM = (
    # PX CM
    (12, 10),
    (17, 15),
    (23, 20),
    (29, 25),
    (36, 30),
    (40, 35),
    (45, 40),
    (50, 45),
    (56, 50),
    (60, 55),
    (65, 60),
    (70, 65),
    (74, 70),
    (77, 75),
    (81, 80),
    (84, 85),
    (86, 90),
    (88, 95),
    (91, 100),
    (92, 105),
    (93, 110),
    (94, 115),
    (95, 120),
    (97, 125),
    (98, 130),
    (99, 135),
    (101, 140),
    (102, 145),
    (103, 150),
    (103, 155),
    (104, 160),
    (104, 165),
    (120, 210))

########################################################################################################################
# ---------------------------------------------------- CODE ---------------------------------------------------------- #

THRESHOLDS = {"yellow": YELLOW_THRESHOLD, "blue": BLUE_THRESHOLD, "ball": RED_THRESHOLD}
COLORS = {"yellow": (255, 255, 0), "blue": (0, 0, 255), "ball": (255, 0, 0)}


def print_settings():
    s = ROBOT
    print("########## copy to settings.py ##########")
    print("YELLOW_THRESHOLD_%s: tuple[list[int]] = %s" % (s, YELLOW_THRESHOLD))
    print("BLUE_THRESHOLD_%s: tuple[list[int]] = %s" % (s, BLUE_THRESHOLD))
    print("RED_THRESHOLD_%s: tuple[list[int]] = %s" % (s, RED_THRESHOLD))
    print()
    print("WHITE_%s = %s" % (s, WHITE))
    print()
    x, y = CENTER_OFFSET
    print("CENTER_%s = maths.Point(FRAME_W_%s // 2 %s %d, FRAME_H_%s // 2 %s %d)"
          % (s, s, "-" if x < 0 else "+", abs(x), s, "-" if y < 0 else "+", abs(y)))
    print("MASK_RADIUS_%s: int = %d" % (s, MASK_RADIUS))
    print("ROBOT_RADIUS_%s: int = %d" % (s, ROBOT_RADIUS))
    print("GAIN_%s = %s" % (s, GAIN))
    print("EXPOSURE_%s = %d" % (s, EXPOSURE))
    print("##########################################")


def init_camera(auto):
    # The same as VisionSystem.init(): auto settings first, then the fixed ones
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_auto_gain(True)
    sensor.set_auto_whitebal(True)
    sensor.set_auto_exposure(True)
    sensor.skip_frames(time=1000)

    if not auto:
        sensor.set_auto_gain(False, gain_db=GAIN)
        sensor.set_auto_whitebal(False, rgb_gain_db=WHITE)
        sensor.set_auto_exposure(False, exposure_us=EXPOSURE)
        sensor.skip_frames(time=1000)


def center():
    return (sensor.width() // 2 + CENTER_OFFSET[0], sensor.height() // 2 + CENTER_OFFSET[1])


def hide_outside(img):
    # The same as VisionSystem.capture_frame(): only the mirror, without the robot itself
    cx, cy = center()
    img.mask_circle(cx, cy, MASK_RADIUS)
    img.draw_circle(cx, cy, ROBOT_RADIUS, color=(0, 0, 0), fill=True)


def linearize(pixels):
    # The same as DistanceCalculator.linearize(): no interpolation, the lower point of the interval
    res = (0, 0)
    for i in range(len(PX_TO_CM)):
        if i < len(PX_TO_CM) - 1:
            if PX_TO_CM[i][0] <= abs(pixels) <= PX_TO_CM[i + 1][0]:
                res = PX_TO_CM[i]
                break
        else:
            res = PX_TO_CM[-1]
    return res[1]


def constrain_angle(angle):
    if angle < -180:
        return angle + 360
    if angle > 180:
        return angle - 360
    return angle


def find(img, name):
    """Biggest goal blob or the roundest ball blob, like ObjectDetector.find_object(). None if not found."""
    blobs = img.find_blobs(THRESHOLDS[name])
    if not blobs:
        return None
    key = (lambda b: b.roundness()) if name == "ball" else (lambda b: b.area())
    blob = max(blobs, key=key)

    cx, cy = center()
    w = blob.cx() - cx
    h = blob.cy() - cy
    px = math.sqrt(w ** 2 + h ** 2)
    return {
        "blob": blob,
        "angle": constrain_angle(math.floor(math.degrees(math.atan2(w, h)) + 180)),
        "px": px,
        "cm": linearize(int(px)),
    }


def draw(img, name, obj):
    blob = obj["blob"]
    color = COLORS[name]
    if name == "ball":
        img.draw_circle(blob.cx(), blob.cy(), max(blob.w(), blob.h()) // 2 + 2, color=color, thickness=2)
    else:
        img.draw_rectangle(blob.rect(), color=color, thickness=2)
    cx, cy = center()
    img.draw_line(cx, cy, blob.cx(), blob.cy(), color=(255, 255, 255), thickness=1)
    img.draw_string(blob.cx() + 4, blob.cy() + 4, "%d %dcm" % (obj["angle"], obj["cm"]), color=color, scale=1)


def describe(name, obj):
    if obj is None:
        return "%-6s  ---" % name
    return "%-6s  angle %4d  dist %3d cm  (%3d px, area %d)" % (
        name, obj["angle"], obj["cm"], obj["px"], obj["blob"].pixels())


def mode_objects(img, show):
    hide_outside(img)
    found = {name: find(img, name) for name in ("yellow", "blue", "ball")}
    for name, obj in found.items():
        if obj:
            draw(img, name, obj)
    if show:
        print(" | ".join(describe(n, found[n]) for n in ("ball", "yellow", "blue")))


def mode_mask(img, show):
    hide_outside(img)
    obj = find(img, TARGET)
    # Pixels inside the threshold become white, everything else black
    img.binary(THRESHOLDS[TARGET])
    if obj:
        img.draw_rectangle(obj["blob"].rect(), color=(255, 0, 0), thickness=2)
    if show:
        print(describe(TARGET, obj), "  threshold", THRESHOLDS[TARGET])


def mode_threshold(img, show):
    x = SAMPLE_POS[0] - SAMPLE_SIZE // 2
    y = SAMPLE_POS[1] - SAMPLE_SIZE // 2
    roi = (x, y, SAMPLE_SIZE, SAMPLE_SIZE)
    hist = img.get_histogram(roi=roi)
    lo = hist.get_percentile(0.02)
    hi = hist.get_percentile(0.98)
    img.draw_rectangle(roi, color=(0, 255, 0), thickness=1)
    img.draw_cross(SAMPLE_POS[0], SAMPLE_POS[1], color=(0, 255, 0), size=3)
    if show:
        # A small margin, otherwise the edges of the object and darker frames drop out
        m = 5
        t = (max(0, lo.l_value() - m), min(100, hi.l_value() + m),
             max(-128, lo.a_value() - m), min(127, hi.a_value() + m),
             max(-128, lo.b_value() - m), min(127, hi.b_value() + m))
        print("%s in the square: L %d..%d  A %d..%d  B %d..%d   ->   %s_THRESHOLD = [%s]" % (
            TARGET, lo.l_value(), hi.l_value(), lo.a_value(), hi.a_value(), lo.b_value(), hi.b_value(),
            "RED" if TARGET == "ball" else TARGET.upper(), t))


def mode_center(img, show):
    cx, cy = center()
    img.draw_circle(cx, cy, MASK_RADIUS, color=(0, 255, 0), thickness=1)
    img.draw_circle(cx, cy, ROBOT_RADIUS, color=(255, 0, 0), thickness=1)
    img.draw_cross(cx, cy, color=(255, 0, 0), size=10)
    if show:
        print("CENTER = (%d, %d)  offset %s  mask %d  robot %d" % (cx, cy, CENTER_OFFSET, MASK_RADIUS, ROBOT_RADIUS))


def mode_distance(img, show):
    hide_outside(img)
    obj = find(img, TARGET)
    if obj:
        draw(img, TARGET, obj)
    if show:
        if obj:
            # px is what goes into the left column of PX_TO_CM
            print("%s: %d px  -> now %d cm by the table" % (TARGET, int(obj["px"]), obj["cm"]))
        else:
            print("%s not found" % TARGET)


def mode_exposure(img, show):
    hide_outside(img)
    if show:
        print("GAIN_%s = %d    WHITE_%s = %s    EXPOSURE_%s = %d" % (
            ROBOT, round(sensor.get_gain_db()), ROBOT,
            tuple(round(v) for v in sensor.get_rgb_gain_db()), ROBOT, sensor.get_exposure_us()))


MODES = {
    "objects": mode_objects,
    "mask": mode_mask,
    "threshold": mode_threshold,
    "center": mode_center,
    "distance": mode_distance,
    "exposure": mode_exposure,
}


def main():
    if MODE not in MODES:
        raise ValueError("MODE must be one of: " + ", ".join(MODES))
    if TARGET not in THRESHOLDS:
        raise ValueError("TARGET must be one of: " + ", ".join(THRESHOLDS))

    print_settings()
    init_camera(auto=(MODE == "exposure"))

    run = MODES[MODE]
    clock = time.clock()
    last_print = time.ticks_ms()
    while True:
        clock.tick()
        img = sensor.snapshot()
        show = time.ticks_diff(time.ticks_ms(), last_print) >= PRINT_EVERY_MS
        run(img, show)
        if show:
            last_print = time.ticks_ms()
            if MODE == "objects":
                print("fps %.1f" % clock.fps())


main()
