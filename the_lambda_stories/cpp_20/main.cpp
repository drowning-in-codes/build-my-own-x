#include <concepts>
#include <iostream>
#include <set>
#include <vector>

template <class First, class... Args>
void captureTest(First &&first, Args &&...args) {
  const auto printer = [first = std::move(first),
                        ... capturedArgs = std::move(args)] {
    std::cout << *first;
    ((std::cout << ", " << capturedArgs), ...);
    std::cout << '\n';
  };
  printer();
}
template <typename T>
concept IRenderable = requires(T v) {
  { v.render() } -> std::same_as<void>;
  { v.getVertCount() } -> std::convertible_to<std::size_t>;
};
struct Circle {
  void render() { std::cout << "drawing circle\n"; }
  std::size_t getVertCount() const { return 1; }
};
struct Square {
  void render() { std::cout << "drawing square\n"; }
  std::size_t getVertCount() const { return 4; }
};
template <typename F> struct Product {
  int _id{0};
  double _price{0.0};
  F predicate;
};
struct Staff {
  std::string _name;
  int _id{0};
};
void TestFunc(int) {}
int main(int argc, char *argv[]) {
  const auto idCmp = [](const auto &a) noexcept { return a.id != 0; };
  Product product{10, 10.0, idCmp};
  auto p2 = product;
  const auto nameCmp = [](const auto &a, const auto &b) {
    return a._name == b._name;
  };
  const std::set<Staff, decltype(nameCmp)> productSet{{"cpu", 1}};
  for (const auto &elem : productSet) {
    std::cout << elem._name << '\n';
  }
  const auto RenderCaller = [](IRenderable auto &shape) {
    shape.render();
    std::cout << shape.getVertCount() << '\n';
  };
  Circle circle;
  RenderCaller(circle);
  // consteval lambda
  auto lam = [](int x) consteval { return x + x; };
  // capture parameter pack
  auto ptr = std::make_unique<int>(10);
  captureTest(std::move(ptr), 2, 3, 4);
  if (ptr == nullptr) {
    std::cout << "moved\n";
  }
  // template lamda
  auto foo = []<typename T>(std::vector<T> const &vec) {
    std::cout << std::size(vec) << '\n';
    std::cout << vec.capacity << '\n';
  };

  auto f = []<typename T>(T const &x) {
    T copy = x;
    T::func();
    using Iterator = typename T::iterator;
  };

  auto ForwardToTestFunc = []<typename... T>(T &&...args) {
    return TestFunc(std::forward<T>(args)...);
  };
  // concepts and lambda

  return 0;
}
