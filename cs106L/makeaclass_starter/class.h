// Blank header file

#include <ostream>
class myConstucor {
public:
  myConstucor() = default;
  myConstucor(int t_x, int t_y);
  int getX() const;
  void setX(int x);
  int getY() const;
  void setY(int y);

  int getMult() const;
  std::ostream &operator<<(std::ostream &os);
  friend std::ostream &operator<<(std::ostream &os, const myConstucor &mc);

private:
  int m_x, m_y;
};
template <typename T> class temConstucor {
  T x, y;
  temConstucor() = default;
  temConstucor(T x, T y) {
    this.x = x;
    this.y = y;
  }
};
