from perception import DataSender


class DataSenderTest:
    def main(self):
        data_sender = DataSender(1, 230400)
        data_sender.init()

        buffer = (400, 400, 400, 400)
        while True:
            # print(data_sender.send(buffer))
            data_sender.send(buffer)


def main():
    test = DataSenderTest()
    test.main()


if __name__ == "__main__":
    main()
