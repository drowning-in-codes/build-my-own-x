#pragma once
#include <concepts>
#include <type_traits>
template <typename T> T maxValue(T a, T b) { return a > b ? a : b; }
// 概念是一个模板，应用于传递的模板参数的一个或多个需求引入名称，以便可以将这些需求用
// 作约束。
// 定义概念
template <typename T>
concept IsPointer = std::is_pointer_v<T>;
//
// template <typename T>
//   requires(!std::is_pointer_v<T>)
// T maxValue(T a, T b) {
//   return a > b ? a : b;

// 重载解析认为有约束的模板比没有约束的模板更特化，所以只约束对指针实现就够了
// template <typename T>
//   requires IsPointer<T>
// auto maxValue(T a, T b) {
//   return maxValue(*a, *b);
// }

// 类型约束
// 若约束是应用于参数的单个概念，则有几种方法可以简化约束的说明。
// 1. 可以在声明模板形参时，直接将其指定为类型约束
template <IsPointer T> auto maxValue(T a, T b) { return maxValue(*a, *b); }

// 不使用模板语法
// 2. 使用auto声明形参，可以将概念用作类型约束
auto maxValue(IsPointer auto a, IsPointer auto b) { return maxValue(*a, *b); }
// 3. 可以加上const和引用
auto maxValue(const IsPointer auto &a, const IsPointer auto &b) {
  return maxValue(*a, *b);
}

// 使用模板语法
// template <IsPointer T1, IsPointer T2> auto maxValue(T1 a, T2 b) {
//   return maxValue(*a, *b);
// }
// 还应该允许比较值的基本函数模板使用不同的类型
//
// template <typename T1, typename T2> auto maxValue(T1 a, T2 b) {
//   return a > b ? a : b;
// }
//  声明要求指针所指向的值具有可比性
// 用于检查传递的类型的值是否支持可比较操作符 ==、!=、<、<=、> 和 >=
auto maxValue(IsPointer auto a, IsPointer auto b)
  requires std::totally_ordered_with<decltype(*a), decltype(*b)>
{
  return maxValue(*a, *b);
}
// 上面的模板只适合原始指针，不适用于智能指针
template <typename T>
concept IsPointerWithSmart = requires(T t) { *t; };
// 表达式 *p 必须对类型 T的对象 p 有效。
template <typename T>
concept IsComplexPointer = requires(T t) {
  *t;                              //  operator * has to be valid
  t == nullptr;                    // can compare with nullptr
  { t < t } -> std::same_as<bool>; //  operator < yields bool
};
// 该类型必须支持解引用操作符。
// 该类型必须支持小于操作符，该操作符必须产生 bool 类型。
// 该类型的对象必须与nullptr 比较。
