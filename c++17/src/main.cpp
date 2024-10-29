#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

template <typename T>
void printElems(const T& coll, std::string_view prefix = {}) {
  for (const auto& elem : coll) {
    if (prefix.data()) {
      std::cout << prefix;
    }
    std::cout << elem << '\n';
  }
}
std::optional<int> asInt(std::string_view sv) {
  int val;
  auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
  if (ec != std::errc{}) {
    // 有错误返回nullopt
    return std::nullopt;
  }
  return val;
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
    auto oi = asInt(s);
    if (oi) {
      std::cout << "Success: " << *oi << '\n';
    } else {
      std::cout << "Failed\n";
    }
  }
  Person p{"nico"};
  std::string s = "Joe";
  Person p2{s};
  Person p3{std::move(s)};
  return 0;
}