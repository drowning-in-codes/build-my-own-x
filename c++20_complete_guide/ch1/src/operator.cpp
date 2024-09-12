#include <algorithm>
#include <compare>
#include <cstdio>
#include <iostream>
#include <vector>
class Value {
private:
  long id;

public:
  bool operator==(const Value &rhs) const { return id == rhs.id; }
  bool operator!=(const Value &rhs) const {
    return !(*this == rhs); // derived check
  }
  bool operator<(const Value &rhs) const { return id < rhs.id; }
  bool operator<=(const Value &rhs) const {
    return !(rhs < *this); // derived check
  }
  bool operator>(const Value &rhs) const { return id > rhs.id; }
  bool operator>=(const Value &rhs) const {
    return !(rhs > *this); // derived check
  }
  long getId() const { return id; }
};

// 若操作符不能抛出，就用 noexcept 声明
// 若操作符可以在编译时使用，则使用 constexpr 声明
// 若构造函数不是显式的，则将操作符声明为“隐藏友元”(在类结构中与友元一起声明，以便
// 两个操作数都成为参数，并支持隐式类型转换) 在下面的例子中
// 可以有比较有Value_(2)<1,因为支持隐式构造类型转换
// 声明带有 [[nodiscard]]的操作符，以便在返回值未使用时强制发出警告
class Value_ {
private:
  long id;

public:
  constexpr Value_(long i) noexcept : id{i} {}
  [[nodiscard]] friend constexpr bool operator==(const Value_ &lhs,
                                                 const Value_ &rhs) noexcept {
    return lhs.id == rhs.id;
  }
  [[nodiscard]] friend constexpr bool operator!=(const Value_ &lhs,
                                                 const Value_ &rhs) noexcept {
    return !(lhs.id == rhs.id);
  }
  [[nodiscard]] friend constexpr bool operator>(const Value_ &lhs,
                                                const Value_ &rhs) noexcept {
    return lhs.id > rhs.id;
  }
  [[nodiscard]] friend constexpr bool operator>=(const Value_ &lhs,
                                                 const Value_ &rhs) noexcept {
    return !(rhs.id > lhs.id);
  }
  [[nodiscard]] friend constexpr bool operator<(const Value_ &lhs,
                                                const Value_ &rhs) noexcept {
    return lhs.id < rhs.id;
  }
  [[nodiscard]] friend constexpr bool operator<=(const Value_ &lhs,
                                                 const Value_ &rhs) noexcept {
    return !(rhs.id < lhs.id);
  }
  long getId() { return id; }
};

// 上面都是c++20之前的操作
//
class ModernValue {
private:
  long id;

public:
  friend constexpr bool operator==(const ModernValue &lhs,
                                   const ModernValue &rhs) {
    return lhs.id == rhs.id;
  }
  friend constexpr auto operator<=>(const ModernValue &lhs,
                                    const ModernValue &rhs) {
    return lhs.id <=> rhs.id;
  }
};
class Person {
  std::string name;
  double value;
  constexpr auto operator<=>(const Person &rhs) const {
    auto cmp1 = name <=> rhs.name;
    if (cmp1 != 0)
      return cmp1;
    return std::strong_order(value, rhs.value);
  }
};
int main() {
  Value v1, v2;
  if (v1 < v2) {
    printf("v2 id is %ld\n", v2.getId());
  }
  std::vector<Value> coll;
  std::sort(coll.begin(), coll.end()); // 基于操作符<
  std::ranges::sort(coll);             // 基于操作符<
  Value_ v3{3};
  if (v3 > 1) {
    printf("v3 id is %ld\n", v3.getId());
  }
}
