#include <concepts>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vcruntime_typeinfo.h>
#include <vector>
// 概念是一个模板，应用于传递的模板参数的一个或多个需求引入名称，以便可以将这些需求用
// 作约束。
template <typename T>
concept IsPointer = requires(T p) {
  *p;
  p == nullptr;
  { p < p } -> std::convertible_to<bool>;
};

auto maxValue(auto a, auto b) { return b < a ? a : b; }
auto maxValue(IsPointer auto a, IsPointer auto b)
  requires std::totally_ordered_with<decltype(*a), decltype(*b)>
{
  return maxValue(*a, *b);
}

// 约束别名模板
template <std::ranges::range T> using ValueType = std::ranges::range_value_t<T>;

// 约束变量模板
template <std::ranges::range T>
constexpr bool IsIntegralValType = std::integral<std::ranges::range_value_t<T>>;

template <typename T> class ValOrColl {
  T value;

public:
  ValOrColl(const T &val) : value(val) {}
  ValOrColl(T &&val) : value(std::move(val)) {}
  void print() const { std::cout << value << '\n'; }

  //  若类型 T 是一个集合，则满足约束，所以两个 print()
  //  成员函数都可用。但重载解析首选这个print()，因为该成员函数有约束
  void print() const
    requires std::ranges::range<T>
  {
    for (const auto &elem : value) {
      std::cout << elem << ' ';
    }
    std::cout << '\n';
  }
};

// 约束非类型模板参数
template <auto Val>
concept Lessthan10 = Val < 10;

template <typename T, int Size>
  requires Lessthan10<Size>
class MyType {};

template <typename Coll>
concept SupportsPushBack = requires(Coll coll) {
  coll.push_back(std::declval<std::ranges::range_value_t<Coll>>());
  // coll.push_back(std::declval<typename Coll::value_type &>());
};

//  不必在这里使用 typename 来使用 Coll::value_type。从 C++20
//  开始，当上下文明确限定成员必
// 须是类型时，不再需要 typename
// requires(Coll c, typename Coll::value_type v) { c.push_back(v); };

template <typename Coll, typename T>
  requires SupportsPushBack<Coll> &&
           std::convertible_to<
               T, typename Coll::
                      value_type> // 检查可以使用push_back并且可以通过复制初始化
void add(Coll &coll, const T &val) {
  coll.push_back(val);
}
// 插入多个值
template <SupportsPushBack Coll, std::ranges::input_range T>
void add(Coll &coll, const T &val) {
  coll.insert(coll.end(), std::ranges::begin(val), std::ranges::end(val));
}

// 插入多个值
template <SupportsPushBack Coll, std::ranges::input_range T>
void insert(Coll &coll, const T &val) {
  coll.insert(std::ranges::begin(val), std::ranges::end(val));
}
template <typename Coll, typename T>
  requires SupportsPushBack<Coll>
void insert(Coll &coll, const T &val) {
  coll.insert(val);
}
// 类型 Coll 必须支持 push_back()。
// 必须进行从类型 T 到 Coll 元素类型的转换。
// 若传递的实参具有元素类型 Coll，则该类型必须支持复制 (使用传递值初始化)。
template <typename Coll, typename T> void add(Coll &coll, const T &val) {
  coll.push_back(val);
}
// 重载解析有一些通用规则，例如:
// 没有类型转换的调用优先于具有类型转换的调用。
// 调用普通函数优于调用函数模板
//
// 重载解析也更喜欢有约束的重载或特化，而不是那些约束较少或没有约束的重载或特化
template <typename Coll, typename T>
  requires std::is_floating_point_v<T>
void add(Coll &coll, const T &val) {
  std::cout << "for double" << std::endl;
  coll.push_back(val);
}
// 窄化限制
template <typename From, typename To>
concept ConvertsWithoutNarrowing =
    std::convertible_to<From, To> && requires(From &&x) {
      {
        std::type_identity_t<To[]>{std::forward<From>(x)}
      } -> std::same_as<To[1]>;
    };

template <typename Coll, typename T>
  requires ConvertsWithoutNarrowing<T, typename Coll::value_type> &&
           SupportsPushBack<Coll>
void add(Coll &coll, const T &val) {
  coll.push_back(val);
}

template <typename Coll, typename T>
  requires SupportsPushBack<Coll> && std::ranges::input_range<T> &&
           ConvertsWithoutNarrowing<std::ranges::range_value_t<T>,
                                    typename Coll::value_type>
void add(Coll &coll, const T &val) {
  coll.insert(coll.end(), std::ranges::begin(val), std::ranges::end(val));
}

// 包含约束
// 概念可以包含其他概念，多以更特化地用于重载解析

template <typename T> using ValType = typename T::value_type;
// 概念可以约束几乎所有形式的泛型代码，包括函数模板、类模板、成员函数模板、成员
// 但不能约束概念
int main() {
  int x = 42;
  int y = 77;
  [[maybe_unused]]
  bool b1 = IsIntegralValType<std::vector<int>>;
  std::cout << maxValue(x, y) << std::endl;
  std::cout << maxValue(&x, &y) << std::endl;
  int *xp = &x;
  int *yp = &y;
  std::cout << maxValue(xp, yp) << std::endl;

  [[maybe_unused]]
  ValueType<std::vector<int>> v2 = 12;
  double d = 49.9;
  std::cout << maxValue(xp, &d) << std::endl;
  ValOrColl o1 = 42;
  o1.print();
  ValOrColl o2 = std::vector{1, 2, 3, 4, 5};
  o2.print();
}
