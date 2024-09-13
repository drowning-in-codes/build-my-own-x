#pragma once
#include <type_traits>
template <typename T> T maxValue(T a, T b) { return a > b ? a : b; }
// 概念是一个模板，应用于传递的模板参数的一个或多个需求引入名称，以便可以将这些需求用
// 作约束。
template <typename T>
concept IsPointer = std::is_pointer_v<T>;

template <typename T>
  requires(!IsPointer<T>)
T maxValue(T a, T b) {
  return a > b ? a : b;
}
