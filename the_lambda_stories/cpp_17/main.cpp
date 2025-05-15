#include <array>
#include <iostream>

template <typename Range, typename Func, typename T>
constexpr T SimpleAccumulate(Range &&range, Func func, T init) {
  for (auto &&elem : range) {
    init += func(elem);
  }
  return init;
};
constexpr int add(int const &t, int const &u) { return t + u; }

struct Baz {
  auto foo() {
    return [*this]() { std::cout << s << '\n'; };
  }
  auto bar() const {
    return [s = s]() { std::cout << s << '\n'; };
  }
  std::string s;
};
int main() {
  // constexpr lambda
  constexpr std::array arr{1, 2, 3};
  constexpr auto sum = SimpleAccumulate(arr, [](auto i) { return i * i; }, 0);
  static_assert(sum == 14);
  constexpr int x = 0;
  constexpr auto lam = [x](int n) { return add(x, n); };
  static_assert(lam(10) == 10);

  // capture *this
  const auto f1 = Baz{"xyz"}.foo();
  f1();

  const auto f2 = Baz{"xyz"}.bar();
  f2();
  int num{5};
  const auto var = std::invoke([num] {
    std::string greetings;
    for (std::size_t i{}; i < num; i++) {
      greetings += "Hi ";
    }
    return greetings;
  });
  // Variadic Generic Lambdas
  const auto sumLambda = [](auto... args) {
    std::cout << sizeof...(args) << '\n';
    return (args + ... + 0);
  };
  std::cout << sumLambda(1.1, 2.2, 3);
  const auto printer = [](auto... args) { (std::cout << ... << args) << '\n'; };
  const auto printer_2 = [](auto... args) {
    const auto printElem = [](auto elem) { std::cout << elem << ' '; };
    (printElem(args), ...);
  };
  return 0;
}
