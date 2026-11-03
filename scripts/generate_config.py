"""Generates include/config.h from config.json.

The robot can't read JSON, so the settings are turned into a C++ header before every build.
PlatformIO runs this script itself (extra_scripts in platformio.ini), the simulator calls it from sim/build.py.
By hand: python scripts/generate_config.py
"""

import json
import sys
from pathlib import Path

PIN_NAMES = ["phototransistor", "btn_1", "btn_2", "btn_3", "m1_1", "m1_2", "m2_1", "m2_2", "m3_1", "m3_2",
             "m4_1", "m4_2", "solenoid", "dribbler", "led", "distance_sensor"]
MOTOR_ANGLES = (45, 135, 225, 315)
MOTOR_DIRECTIONS = ("forward", "backward")

HEADER = """\
#pragma once

// GENERATED from config.json by scripts/generate_config.py, don't edit: change config.json instead

#include <stdint.h>

#include <Arduino.h>

#include "utils/control/PID-control/PID-control.h"
#include "utils/control/PDC-control/PDC-control.h"
#include "control/outs-handler-coords/outs-handler-coords.h"
#include "control/movement-controller/movement-controller.h"
#include "management/robot-type/robot-type.h"
#include "core/UART/UART.h"
"""


class ConfigError(Exception):
    pass


def _get(section: dict, key: str, where: str):
    if key not in section:
        raise ConfigError(f"{where}: missing \"{key}\"")
    return section[key]


def _number(value, where: str) -> str:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ConfigError(f"{where}: expected a number, got {value!r}")
    return repr(value)


def _float(value, where: str) -> str:
    _number(value, where)
    return repr(float(value))


def _robot(name: str, cfg: dict) -> str:
    up = name.upper()
    cap = name.capitalize()
    out = []

    def line(s=""):
        out.append(s)

    line("/" * 127)
    line(f"// {' ' + up + ' ':-^123} //")
    line(f"#define SPEED_{up} {_number(_get(cfg, 'speed', name), f'{name}.speed')}")
    line()

    pins = _get(cfg, "pins", name)
    line(f"enum class Pin{cap} : uint8_t")
    line("{")
    for i, pin in enumerate(PIN_NAMES):
        value = _get(pins, pin, f"{name}.pins")
        # Analog pins are written as "A14", Arduino.h defines them
        if not (isinstance(value, str) and value[:1] == "A" and value[1:].isdigit()):
            value = _number(value, f"{name}.pins.{pin}")
        line(f"    PIN_{pin.upper()} = {value}{',' if i < len(PIN_NAMES) - 1 else ''}")
    line("};")
    line()

    uart = _get(cfg, "uart", name)
    num = _get(uart, "num", f"{name}.uart")
    if num not in (1, 2, 3):
        raise ConfigError(f"{name}.uart.num: expected 1, 2 or 3, got {num!r}")
    line(f"const UARTNum UART_num_{name} = UARTNum::UART_{num};")
    for key in ("baudrate", "buffer_size", "buffer_starter"):
        line(f"#define UART_{key.upper()}_{up} {_number(_get(uart, key, f'{name}.uart'), f'{name}.uart.{key}')}")
    line()

    outs = _get(cfg, "outs_bounds", name)
    line("// cm, field coordinates: +x to the right, +y towards the opponent goal")
    line(f"enum class OutsBounds{cap} : int16_t")
    line("{")
    keys = ("right", "left", "front_goal", "rear_goal", "front_corners", "rear_corners", "goal_zone_half_width")
    for i, key in enumerate(keys):
        value = _get(outs, key, f"{name}.outs_bounds")
        if not isinstance(value, int) or isinstance(value, bool):
            raise ConfigError(f"{name}.outs_bounds.{key}: expected a whole number of cm, got {value!r}")
        line(f"    {key.upper()} = {value}{',' if i < len(keys) - 1 else ''}")
    line("};")
    if outs["front_corners"] < outs["front_goal"] or outs["rear_corners"] > outs["rear_goal"]:
        raise ConfigError(f"{name}.outs_bounds: the corners can't be closer to the field center than the bound in front "
                          f"of the goal (front_corners >= front_goal, rear_corners <= rear_goal)")
    hysteresis = _number(_get(cfg, "outs_hysteresis", name), f"{name}.outs_hysteresis")
    line(f"#define OUTS_HYSTERESIS_{up} {hysteresis} // cm back inside a bound before the out is cleared")
    line()

    motors = _get(cfg, "motors", name)
    angles = _get(motors, "location_angles", f"{name}.motors")
    directions = _get(motors, "directions", f"{name}.motors")
    if len(angles) != 4 or any(a not in MOTOR_ANGLES for a in angles):
        raise ConfigError(f"{name}.motors.location_angles: expected 4 values out of {MOTOR_ANGLES}, got {angles}")
    if len(directions) != 4 or any(d not in MOTOR_DIRECTIONS for d in directions):
        raise ConfigError(f"{name}.motors.directions: expected 4 values out of {MOTOR_DIRECTIONS}, got {directions}")
    line(f"const motorsLocAngles motors_loc_angles_{name} = {{")
    line(",\n".join(f"    MotorLocationAngle::ANGLE_{a}" for a in angles) + "};")
    line()
    line(f"const motorsDirections motors_directions_{name} = {{")
    line(",\n".join(f"    MotorDirection::{d}" for d in directions) + "};")
    line()

    address = _get(cfg, "gyroscope_address", name)
    try:
        address = int(address, 0) if isinstance(address, str) else int(address)
    except ValueError:
        raise ConfigError(f"{name}.gyroscope_address: expected a number like \"0x29\", got {address!r}")
    line(f"#define GYROSCOPE_ADDRESS_{up} 0x{address:02X}")
    line()

    pid = _get(cfg, "gyro_align_pid", name)
    pdc = _get(cfg, "gyro_align_pdc", name)
    pid_values = ", ".join(_float(_get(pid, k, f"{name}.gyro_align_pid"), f"{name}.gyro_align_pid.{k}")
                           for k in ("kp", "ki", "kd"))
    pdc_values = ", ".join(_float(_get(pdc, k, f"{name}.gyro_align_pdc"), f"{name}.gyro_align_pdc.{k}")
                           for k in ("kp", "kd", "kc"))
    line(f"const PIDCoefficients control_gyro_align_pid_{name} = {{{pid_values}}};")
    line(f"const PDCCoefficients control_gyro_align_pdc_{name} = {{{pdc_values}}};")
    line()

    # Turning while the dribbler holds the ball: softer, and the turning speed is limited so the ball isn't lost
    with_ball_pdc = _get(cfg, "with_ball_align_pdc", name)
    with_ball_values = ", ".join(_float(_get(with_ball_pdc, k, f"{name}.with_ball_align_pdc"), f"{name}.with_ball_align_pdc.{k}")
                            for k in ("kp", "kd", "kc"))
    with_ball_max = _number(_get(cfg, "with_ball_align_max_speed", name), f"{name}.with_ball_align_max_speed")
    line(f"const PDCCoefficients control_with_ball_align_pdc_{name} = {{{with_ball_values}}};")
    line(f"#define WITH_BALL_ALIGN_MAX_SPEED_{up} {with_ball_max} // PWM, the fastest the robot may turn with the ball")
    line()

    line(f"#define KICKING_TIME_MS_{up} {_number(_get(cfg, 'kicking_time_ms', name), f'{name}.kicking_time_ms')}")
    line()

    dribbler = _get(cfg, "dribbler", name)
    line(f"#define DRIBBLER_MIN_{up} {_number(_get(dribbler, 'min', f'{name}.dribbler'), f'{name}.dribbler.min')}")
    line(f"#define DRIBBLER_MAX_{up} {_number(_get(dribbler, 'max', f'{name}.dribbler'), f'{name}.dribbler.max')}")
    line()

    threshold = _number(_get(cfg, "phototransistor_threshold", name), f"{name}.phototransistor_threshold")
    line(f"#define PHOTOTRANSISTOR_THRESHOLD_{up} {threshold}")
    line()

    offset = _number(_get(cfg, "ball_angle_offset", name), f"{name}.ball_angle_offset")
    line(f"#define BALL_ANGLE_OFFSET_{up} {offset} // deg, subtracted from the camera ball angle")
    return "\n".join(out) + "\n"


def generate(root: Path) -> bool:
    """Returns True if config.h changed. Doesn't touch an up to date file, so nothing gets rebuilt for no reason."""
    src = root / "config.json"
    dst = root / "include" / "config.h"
    try:
        cfg = json.loads(src.read_text())
    except json.JSONDecodeError as e:
        raise ConfigError(f"config.json, line {e.lineno}: {e.msg}")

    text = HEADER + "".join("\n" + _robot(name, _get(cfg, name, "config.json")) for name in ("violet", "white"))
    if dst.exists() and dst.read_text() == text:
        return False
    dst.write_text(text)
    return True


def _main(root: Path) -> None:
    try:
        if generate(root):
            print("config.json -> include/config.h")
    except ConfigError as e:
        sys.exit(f"Error in config.json: {e}")


try:
    Import("env")  # noqa: F821 (PlatformIO: run as an extra script)
    _main(Path(env["PROJECT_DIR"]))  # noqa: F821
except NameError:
    if __name__ == "__main__":
        _main(Path(__file__).resolve().parent.parent)
