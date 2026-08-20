class Point:
    def __init__(self, x: int, y: int) -> None:
        self.__x = x
        self.__y = y

    def x(self) -> float:
        return self.__x

    def y(self) -> float:
        return self.__y

    __x: float
    __y: float
