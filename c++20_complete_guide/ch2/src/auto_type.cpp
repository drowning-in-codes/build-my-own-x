#include "mini_math.hpp"
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdio>
#include <iostream>
#include <map>
#include <numeric>
#include <ostream>
#include <queue>
#include <ranges>
#include <test_math.hpp>
#include <type_traits>
#include <vector>
class MyType {
  // auto用于成员函数的参数
  void assign(const auto &newVal);
  //
  // template<typename T>
  // void assign(cosnt T&)
};

// auto用于普通函数
void printColl_(const auto &coll) {
  for (const auto &elem : coll) {
    puts(elem);
  }
}

// 延迟检查
class C2; // forward declare to use concepts to constrain the type
class C1 {
public:
  // void foo(const auto &c2) const { c2.print(); }
  void foo(const std::same_as<C2> auto &c2) const { c2.print(); }
  void print() const;
};
class C2 {
public:
  void foo(const auto &c1) const { c1.print(); }
  void print() const;
};
int main() {
  // auto用于lambda函数
  auto printColl = [](const auto &coll) {
    for (const auto &elem : coll) {
      std::cout << elem << "\n";
    }
  };
  printColl(std::vector<int>{1, 2, 3});
  return 0;
}
