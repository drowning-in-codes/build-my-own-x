export module Square;
int square(int i);
namespace Square {
export class Square {
private:
  int value;

public:
  Square(int i) : value{square(i)} {}
  int getValue() const { return value; }
};

export template <typename T> Square toSquare(const T &x) { return Square{x}; }
int square(int i) { return i * i; }
} // namespace Square
