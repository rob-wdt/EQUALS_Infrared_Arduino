from typing import LiteralString

import pyb


class DataSender:
    def __init__(self, bus: int, baud: int) -> None:
        self.__uart = pyb.UART(bus)
        self.__baud = baud

    def init(self) -> None:
        self.__uart.init(self.__baud)

    def __clamp_value(self, value) -> int:
        val = int(value / 3)

        if val + 127 > 253:
            return 253
        elif val + 127 < 0:
            return 0
        return val + 127

    def send(self, buffer: tuple[int, ...]) -> str:
        data = bytearray(len(buffer))
        for i in range(len(buffer)):
            data[i] = self.__clamp_value(buffer[i])

        self.__uart.writechar(255)
        to_send = f""

        for i in data:
            to_send += f"{int(i)},"

        self.__uart.write(to_send)

        return to_send

    __uart: pyb.UART
    __baud: int
