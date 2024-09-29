#include <concepts>
#include <iostream>

template <std::ranges::sized_range T> class MyCount {};

// 包含概念
// template <typename T, typename U>
//   requires std::is_same_v<T, U>
// void foo(T, U) {
//   std::cout << "foo(T,U)" << std::endl;
// }
//
// template <typename T, typename U>
//   requires std::is_same_v<T, U> && std::is_integral_v<T>
// void foo(T, U) {
//   std::cout << "foo(T,U) with constraints" << std::endl;
// }
//
template <typename T, typename U>
  requires std::same_as<T, U>
void foo(T, U) {
  std::cout << "foo(T,U) with requires" << std::endl;
}

template <typename T, typename U>
  requires std::same_as<T, U> && std::integral<T>
void foo(T, U) {
  std::cout << "foo(T,U) with requires" << std::endl;
}

int main() { foo(1, 2); }
