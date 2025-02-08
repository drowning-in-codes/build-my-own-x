#include <iostream>
#include <map>
struct Point {
  int x;
  int y;
};

Point getPoint() { return {10, 20}; }
int main() {
  std::map<int, std::string> m;
  Point p1{10, 20};

  auto [x, y]{p1};
  auto [x1, y1]{getPoint()};
  auto [x2, y2]{std::pair{10, 20}};
  for (const auto &[key, val] : m) {
    std::cout << key << ":" << val << std::endl;
  }
  auto [u, v] = p1;

  return 0;
}