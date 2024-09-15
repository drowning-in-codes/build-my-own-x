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

// 带有 auto 参数的函数不同于 Lambda
// 表达式。例如，不能在不指定泛型参数的情况下，传递带 有 auto 作为参数的函数:
bool lessByNameFunc(const auto &c1, const auto &c2) {
  return c1.getName() < c2.getName();
}
// equal to
// template<typename T1,typename T2>
// bool lessByNameFunc(const T1& c1,const T2& c2) {
//   return c1.getNmae() < c2.getName();
// }
struct Person {
  std::string name;
  std::string getName() const { return name; }
};
// #define PRINT(args, ...) printf(args, ##__VA_ARGS__)

// void bar(auto ...args){
// }
//
template <typename T> void foo(auto x, T y, auto z) {
  std::cout << x << y << z;
}
void addValInto(const auto &val, auto &coll) { coll.insert(val); }
template <typename Coll>
  requires std::ranges::random_access_range<Coll>
void addValInto(const auto &val, Coll &coll) {
  coll.push_back(val);
}
template <std::integral T>
void fizz(auto x, T y, std::convertible_to<T> auto z) {
  std::cout << x << y << z;
}
void bar(auto x, auto y) { std::cout << x << y; }
// 既然普通auto函数类似于template,那么差别是什么
void printFunc(const auto &arg) { std::cout << arg << "\n"; }
int main() {
  std::vector<int> coll;
  // addValInto(42,coll);
  // fizz(12,12,"1");
  fizz(12, 12, '1');
  foo<long, char>(64, 65, 'c');
  foo(1, 2, "a");
  bar("a", 1);
  std::vector<Person> persons;
  auto less_by_name_func = [](const Person &c1, const Person &c2) {
    return c1.getName() < c2.getName();
  };
  // 普通函数参数是auto相当于模板函数
  // 没有直接调用函数模板,编译器不能推断模板参数来编译调用
  // 传递函数模板作为形式,显示指定模板形参
  std::sort(persons.begin(), persons.end(), lessByNameFunc<Person, Person>);
  // 而lambda函数是一个没有泛型类型的对象,编译器可以推断模板参数
  std::sort(persons.begin(), persons.end(), less_by_name_func);
  printFunc(42);
  printFunc("hello");
  auto printFunc2 = [](const auto &arg) { std::cout << arg << "\n"; };
  printFunc2.operator()("hello");
  printFunc2.operator()(123);

  // auto用于lambda函数
  auto printColl = [](const auto &coll) {
    for (const auto &elem : coll) {
      std::cout << elem << "\n";
    }
  };
  printColl(std::vector<int>{1, 2, 3});
  return 0;
}
