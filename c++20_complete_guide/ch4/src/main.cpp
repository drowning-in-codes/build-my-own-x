#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <utility>
template <typename T>
  requires(sizeof(T) > 4) && requires { typename T::value_type; } &&
          std::input_iterator<T>

void foo([[maybe_unused]] T _x) {}

template <typename T>
  requires(sizeof(T) > 4) && requires {
    typename T::value_type;
    std::input_iterator<T>;
  }
void foo([[maybe_unused]] T x) {}

// template<typename T>
// requires(sizeof(int)!=sizeof(long))
//
// template<typename T>
// requires(sizeof(T)<=64)
//
// template<typename T,std::size_t Sz>
// requires(Sz>0)
//
// template<typename T>
// requires(std::is_pointer_v<T> || std::same_as<T,std::nullptr_t>)
//
// template<typename T>
// requires (!std::convertible_to<T,std::string>)
//
// 解引用操作符通常产生一个非整型的引用，所以:
// – 假设有一个类型为 T 的对象:std::declare<T>()
// – 为对象调用解引用操作符:*
// – 获取其类型:decltype()
// – 删除引用:std::remove _reference_v<>
// – 检查是否为整型:std::integral<>
// template<typename T>
// requires std::integral<std::remove_reference_t<decltype(*std::declval<T>())>>
//
// 表达式以 requires 开头，后跟一个可选的参数列表，然后是一个需求块
// (都以分号结束)。 template <typename Coll> requires {
//   typename Coll::value_type::first_type;
//   typename Coll::value_type::second_type;
// };
// // 可选参数列表允许引入一组“虚拟变量”，可用于在表达式的主体中表达需求
// template <typename T> requrires(T x, T Y) {
//   x + y;
//   x - y;
// }
//
// // 这些形参永远不会由实参取代，所以通过值或引用声明都可以
// template <typename Coll>
//   requires(Coll::value_type v)
// {
//   std::cout << v;
// }
//
// template<typename Coll>
// requires(Coll::value_type v){
//   true;
// }
//
// template <typename T1, typename T2>
//   requires requires(T1 val, T2 p) { requires *p > val; } ||
//            requires(T2 p) { requires p == nullptr; }
// void func(T1 x, T2 y) {
//   std::cout << x << y << std::endl;
// }
//
// 类型需求
template <typename T1, typename T2>
  requires requires {
    typename T1::value_type;
    typename std::ranges::iterator_t<T1>;
    typename std::common_type_t<T1, T2>;
  }
void sayHi(T1 x, T2 y) {
  std::cout << x << y << std::endl;
}
// 对于所有类型需求，若类型存在但为空，则满足需求。
// 只能检查给定类型的名称 (类名、枚举类型的名称，来自 typedef 或
// using)，不能使用类型检查 其他类型声明
template <typename T>
  requires requires(T &) { true; }
void sayHi(T) {}

template <std::integral T> class MyType1 {};

template <typename T>
  requires requires { typename MyType1<T>; }
void mytype1(T) {}
// template<typename T>
// concept stdHash = requires {
//   typename std::hash<T>;
// };
template <typename T>
concept stdHash = requires { std::hash<T>{}; };
// 复合需求
template <typename T>
  requires requires(T x) {
    { &x } -> std::input_iterator<>;
    { x == x };
    { &x } -> std::input_or_output_iterator;
    { x == x } -> std::convertible_to<bool>;
    { x == x } noexcept;
  }
void func(T x) {
  std::cout << x << std::endl;
}
template <stdHash T> class MyType {
  T val;
};
// template <typename T>
// concept UnrefIntegral = std::integral<std::remove_reference<T>>;
//
// template <typename T>
// concept Check = requires(T p) {
//   { *p } -> UnrefIntegral;
// };
//
//
//
// 嵌套需求
// template <typename T>
// concept DerefAndIndexMatch =
//     requires(T p) { requires std::same_as<decltype(*p), decltype(p[0])>; };

// 简单需求  必须进行编译。调用不会执行，从而操作产生的结果并不重要
template <typename T1, typename T2>
  requires requires(T1 val, T2 p) {
    *p > val;
    p[0];
    p->value();
    *p > val;
  }
void hi();

template <typename T>
concept DerefAndIndexMatch =
    std::same_as<decltype(*std::declval<T>()), decltype(std::declval<T>()[0])>;

template <typename T>
concept Check = requires(T p) {
  requires std::integral<std::remove_cvref_t<decltype(*p)>>;
};

// 嵌套需求可用于在 requires 表达式中指定附加约束。以 requires
// 开头，后跟一个编译时布尔表
//  达式，该表达式本身也可能是或使用 requires 表达式
template <typename T>
  requires requires {
    !std::is_const_v<T>;
    requires !std::is_const_v<T>;
  }

// 复合需求允许将简单需求和类型需求结合起来，可以指定一个表达式
// (大括号内)，然后添加以 下一个或两个: • noexcept 要求表达式保证不抛出异常 • ->
// type-constraint 将概念应用于表达式的求值
class Teacher {};
// 简单的需求只检查需求是否有效，而不检查需求是否满足。出于这个原因:
//  使用总是产生值的类型函数没有意义:
// 使用产生类型的类型函数没有意义
// 使用可能具有未定义行为的类型函数也没有意义
int main() {
  mytype1(42);
  [[maybe_unused]]
  int x = 42;
  int y = 43;
  [[maybe_unused]]
  int *q = &y;
  // func(x, q);
  //

  // MyType<Teacher> mt;
}
