#include <concepts>
#include <ranges>
#include <type_traits>

// 算法概念
//
template <typename T>
  requires std::integral<T> && std::signed_integral<T> &&
           (sizeof(T) > 4) && std::is_floating_point_v<T> &&
           std::signed_integral<T> // 编译时布尔表达式与概念
void sayHi([[maybe_unused]] T val) {}

// 对象概念
template <typename T>
  requires std::movable<T> && std::copyable<T>
void saiHi([[maybe_unused]] T val) {
} // 保证类型 T
  // 是可移动和可交换的。可以移动构造、移动赋值，以及与该类型的另一个对象交换

// 概念间的组合
template <typename T1, typename T2>
  requires std::same_as<T1, T2>
void sayHi() {}

// std::convertible_to<T1,T2> 用于检查 T1 是否可以隐式转换为 T2
//
// 比较概念

template <typename T, typename U>
  requires std::equality_comparable_with<T, U>
void sayHi() {}

// 范围和视图概念
template <typename T>
  requires std::ranges::range<T>
void sayHi() {}
template <typename T>
  requires std::ranges::input_range<T>
void sayHi() {}

// 几乎所有的概念都定义在命名空间 std 中，唯一的例外是 range
// 概念，它定义在命名空间 std::ranges 中。
int main() { return 0; }
