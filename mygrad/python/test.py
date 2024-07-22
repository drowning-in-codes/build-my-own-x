from typing import TypeAlias, Union, TypeVar, Generic

Number: TypeAlias = Union[int, float, complex]
T = TypeVar("T", bound=Number)


class Value(Generic[T]):
    def __init__(self, value: T = 0) -> None:
        self.data: T = value

    def __add__(self, other):
        if isinstance(other, Number):
            return Value(self.data + other)
        elif isinstance(other, Value):
            return Value(self.data + other.data)
        else:
            raise NotImplementedError("dont support this type %s", type(other))

    def __radd__(self, other):
        return self.__add__(other)

    def __repr__(self) -> str:
        return f"Value:{self.data}"

    def __str__(self) -> str:
        return f"Value:{self.data}"


if __name__ == "__main__":
    a = Value(2)
    b = Value(2)
    s = set()
    s.add(a)
    s.add(b)
    print(s)
    t = set()
    t.add((3, 4))
    print(set((3, 4, 13, 3, 1)))
