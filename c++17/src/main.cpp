#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

template <typename T>
void printElems(const T &coll, std::string_view prefix = {}) {
  for (const auto &elem : coll) {
    if (prefix.data()) {
      std::cout << prefix;
    }
    std::cout << elem << '\n';
  }
}

class Person {
  std::string name;

public:
  Person(std::string /* not string_view */ n) : name{n} {}
};
int main() {
  //   printElems(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, "vec: ");
  for (auto s : {"42", "  077", "hello", "0x33"}) {
    std::cout << "Parsing '" << s << "': ";
    Person p{"nico"};
    Person p2{s};
    Person p3{std::move(s)};
    return 0;
  }
}