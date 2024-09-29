#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>

template <typename T, std::size_t Sz> void printSpan(std::span<T, Sz> sp) {
  for (const auto &elem : sp) {
    std::cout << '"' << elem << "\"";
  }
  std::cout << '\n';
}

// span 指的是任何元素类型的数组，作
// 为原始指针和大小的组合，提供了通常的集合接口，用于读取和写入存储在连续内存中的元素

// 通过要求 span
// 只能引用连续内存中的元素，迭代器可以是原始指针，这使得其很廉价。该集合
// 提供随机访问
// (以便跳转到范围内的任何位置)，因此可以使用此视图对元素进行排序，或者可以使
// 用生成位于底层范围中间或末尾的 n 个元素的子序列的操作。
//
//
//
// 无论通过值传递还是通过 const 引用传递 span，只要没有将元素声明为 const，在函
// 数内部仍可以修改元素。这就是为什么将 span 的元素声明为 const 有意义
int main() {
  // 分配不同的子序列
  std::vector<std::string> vec_str{"one", "two", "three", "four", "five"};
  std::span<const std::string> sp_s{vec_str.data(), 3};

  sp_s = std::span{vec_str.end() - 4, vec_str.end()};
  sp_s = std::span{vec_str}.last(4);
  [[maybe_unused]]
  auto v = std::views::counted(vec_str.begin() + 1, 3);

  auto oldCapa = vec_str.capacity();
  vec_str.push_back("Cairo");
  if (oldCapa != vec_str.capacity()) {
    sp_s = std::span<std::string, 3>{vec_str.data(), 3};
  }
  sp_s = std::span<const std::string, 3>{vec_str.end() - 3, vec_str.end()};
  sp_s = std::span{vec_str}.last<3>();

  std::ranges::sort(std::span{vec_str}.subspan(1, 3));
  // 类型擦除
  std::array arr{1, 2, 3};
  std::vector vec_3{1, 2, 3};
  std::span<int> vecSpanDyn{vec_3};
  std::span<int> arrSpanDyn{arr};
  // span 使用指向内存的原始指针执行元素访问，span
  // 类型会擦除元素存储位置的信息。指向 vector 元素的 span
  // 操作与指向数组元素的span 操作具有相同的类型
  std::cout << std::same_as<decltype(arrSpanDyn), decltype(vecSpanDyn)>;
  // span 的类模板参数推导从数组推导出固定区段，从 vector 推导出动态区段
  std::span<const std::string, 5> vecSpan5{vec_str};
  std::cout << std::same_as<decltype(vecSpan5), decltype(arrSpanDyn)> << '\n';

  int a5[] = {1, 2, 3, 4, 5};
  //   指定元素为 const std::string 时，就不能通过 span 进行修改。将 span
  //   本身声明为 const，并不提
  // 供对引用元素的只读访问
  std::span<const std::string> sp{vec_str.data(), 3};
  printSpan(sp);

  std::ranges::sort(vec_str);
  printSpan(sp);
  // 使用原始数组或 std::array 直接创建和初始化
  // span，由于元素数量不合法而导致的一些 运行时错误将成为编译时错误

  int raw[10]{};
  std::span<int> sp8a{raw};
  std::span<int, 10> sp8c{raw};

  // auto oldCapa = vec_str.capacity();
  vec_str.push_back("six");
  if (oldCapa != vec_str.capacity()) {
    sp = std::span{vec_str.data(), 3};
  }
  // span 必须具有它们引用的序列元素的元素类型。不支持转换
  // (甚至是隐式的标准转换)，但允许 使用 const 等其他限定符

  std::vector<int> vec_4{1, 2, 3, 4};
  std::span<const int> sp9a{vec_4};

  std::vector<long> vec_5{1, 2, 3, 4};
  std::span<long> spo6b{vec_5};

  std::vector<int> vec_6{1, 2, 3, 4};
  std::span<int> spanDyn{vec_6.begin(), 5};
  // std::span<int, 5> spanDyn_2{vec_6.begin(), 6}; // runtime error
  //
  auto sp5 = spanDyn.first(2);
  sp5 = spanDyn.first<2>();
  sp8c = spanDyn.first<10>();

  // 构造函数还允许在 span 之间进行以下类型转换:
  // • 具有固定区段的 span 将转换为具有相同的固定区段和附加限定符的 span。
  // • 具有固定区段的 span 转换为具有动态区段的 span。
  // • 若当前区段适合，则具有动态区段的 span 将转换为具有固定区段的 span。
  // 使用条件显式时，只有固定范围
  // span的构造函数是显式的。若必须转换初始值，则无法进行复 制初始化
  printSpan(sp);
  sp = vec_str;
  printSpan(sp);
  sp = std::span{vec_str.end() - 5, vec_str.end()};
  printSpan(sp);
  sp = std::span{vec_str}.last(4);
  printSpan(sp);
  std::vector<std::string> vec{"hi", "world"};
  [[maybe_unused]]
  std::span<const std::string> sp2{vec.data(), 1};

  // 由于模板类型推导的限制，不能将这样的容器传递给期望使用 span 的函数

  std::array arr5{1, 2, 3, 4, 5};

  [[maybe_unused]]
  std::span sp1 = a5;
  std::span sp2_{arr5};

  [[maybe_unused]]
  std::span sp3 = arr5;
  std::span sp4{vec};
  std::span sp5_{vec.data(), 4};
  // 需要由开发者保证 span 指向一个具有足够元素的有效范围
  std::span sp6{a5 + 1, 3};

  return 0;
}
