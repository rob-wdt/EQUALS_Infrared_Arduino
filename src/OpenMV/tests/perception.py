from typing import NoReturn

from OpenMV.perception import DataSender


class DataSenderTest:
    def main(self) -> NoReturn:
        data_sender = DataSender(3, 230400)
        data_sender.init()

        buffer = (400, 400, 400, 400)
        while True:
            print(*data_sender.send(buffer))


def main() -> NoReturn:
    test = DataSenderTest()
    test.main()


if __name__ == "__main__":
    main()
