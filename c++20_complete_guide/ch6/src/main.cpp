#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>
// 为命名空间引入快捷方式
namespace rg = std::ranges;

template <rg::random_access_range R, typename Comp = std::ranges::less>
  requires std::sortable<std::ranges::iterator_t<R>, Comp>
void sort(R &&r, Comp comp = {});

void print(const auto &coll) {
  for (const auto &elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << '\n';
}
struct NullTerm {
  constexpr bool operator==(auto pos) const { return *pos == '\0'; }
};

template <auto End> struct EndValue {
  bool operator==(auto pos) const { return *pos == End; }
};
// 通用范围代码
// 为了方便针对所有不同类型的范围进行编程，范围库提供了以下工具:
// • 泛型函数，生成迭代器或范围的大小
// • 类型函数，生成迭代器的类型或元素的类型
template <std::ranges::input_range Range>
std::ranges::range_value_t<Range> maxValue(const Range &rg) {
  if (std::ranges::empty(rg)) {
    return std::ranges::range_value_t<Range>{};
  }
  auto pos = std::ranges::begin(rg);
  auto max = *pos;
  while (++pos != std::ranges::end(rg)) {
    if (*pos > max) {
      max = *pos;
    }
  }
  return max;
}
template <std::ranges::input_range Rg, typename T,
          typename Proj = std::identity>
constexpr std::ranges::borrowed_iterator_t<Rg> find(Rg &&r, const T &,
                                                    Proj proj = Proj{});

// 投影
// 可选的附加参数，允许在算法进一步处理之前为每个元素指定一个转换 (投影)
template <std::ranges::random_access_range R, typename Comp = std::ranges::less,
          typename Proj = std::identity>
  requires std::sortable<std::ranges::iterator_t<R>, Comp, Proj>
void sort([[maybe_unused]] R &&r, [[maybe_unused]] Comp comp = Comp{},
          [[maybe_unused]] Proj proj = Proj{}) {};

int main() {
  std::vector<int> coll{
      1,
      2,
      3,
  };
  // 默认的投影是 std::identity()，只产生传递给它的参数，因此根本不执行投影/转换
  std::ranges::sort(coll, std::ranges::less{},
                    [](auto val) { return std::abs(val); });
  // 范围的开始和计数
  // std::views::counted(pos5, 3) 创建了一个视图，该视图表示从 pos5
  // 所引用的元素开始的三个元素。注意，counted() 不会检查是否存在元素
  // (传递的计数过高会导致未定义行为)，开发者要确
  // 保代码的有效性。因此，需要使用
  // std::ranges::distance()，检查是否有足够的元素 (注意，若集合没
  // 有随机访问迭代器，这种检查的开销可能会很大)。
  auto pos5 = std::ranges::find(coll, 5);
  if (std::ranges::distance(pos5, coll.end()) >= 3) {
    for (int val : std::views::counted(pos5, 3)) {
      std::cout << val << " ";
    }
  }
  // 只有当你确实有一个迭代器和一个计数时，才可以使用
  // counted()。若已经有了一个范围， 并且只想处理前 n 个元素，请使用
  // std::views::take()。
  std::ranges::subrange range{coll.begin(), EndValue<7>{}};
  std::sort(coll.begin(), coll.end());
  rg::sort(coll);
  for (const auto &elem : coll | std::views::take(2)) {
    std::cout << elem;
  }
  std::ranges::for_each(range, [](auto val) { std::cout << ' ' << val; });
  std::ranges::for_each(coll.begin(), EndValue<-1>{},
                        [](auto val) { std::cout << ' ' << val; });
  // 这种嵌套很难阅读和维护，可以从另一种管道语法中让视图对范围进行操作。通过使用操作符
  // |，可以创建视图的管道
  [[maybe_unused]]
  auto v = std::views::take(
      std::views::transform(
          std::views::filter(coll, [](auto elem) { return elem % 3 == 0; }),
          [](auto elem) { return elem * elem; }),
      2);
  [[maybe_unused]]
  auto vv = coll | std::views::filter([](auto elem) { return elem % 3 == 0; }) |
            std::views::transform([](auto elem) { return elem * elem; }) |
            std::views::take(2);

  std::map<std::string, int> composers{{"Johann Sebastian Bach", 1685},
                                       {"Ludwig van Beethoven", 1770},
                                       {"Richard Wagner", 1813},
                                       {"Claude Debussy", 1862}};

  for (const auto &elem : composers | std::views::filter([](const auto &y) {
                            return y.second > 1800;
                          }) | std::views::take(3) |
                              std::views::keys) {
    std::cout << "- " << elem << '\n';
  }

  for (int val : std::views::iota(1, 11)) {
    std::cout << val << " ";
  }
  auto v1 = std::views::take(coll, 5);
  auto v2 = coll | std::views::take(5);
  for (int val : v1) {
    std::cout << val << " ";
  }
  std::ranges::sort(v2);

  [[maybe_unused]]
  auto v3 = coll | std::views::take(5);

  // 延迟计算
  auto pos = v3.begin();
  // 哨兵
  std::for_each(std::istream_iterator<int>{std::cin},
                std::istream_iterator<int>{},
                [](int val) { std::cout << val << '\n'; });
  const char *rawString = "hello world";
  // 直接使用哨兵
  for (auto ipos = rawString; ipos != NullTerm{}; ++ipos) {
    std::cout << ' ' << *ipos;
  }

  for (auto ipos = rawString; ipos != NullTerm{}; ++pos) {
    std::cout << *ipos;
  }
  // 将哨兵传递给算法
  // C++20 为不再要求开始迭代器和哨兵 (结束迭代器)
  // 具有相同类型的算法提供了重载，但这些 重载会在命名空间 std::ranges 中提供
  std::ranges::for_each(rawString, NullTerm{},
                        [](auto elem) { std::cout << elem << std::endl; });

  int rawArray[] = {1, 2, 3, 4};
  std::ranges::sort(rawArray);
  // use a null terminator to define a range of a raw string
  std::ranges::subrange rawStringRange{rawString, NullTerm{}};

  std::ranges::for_each(rawStringRange, [](char c) { std::cout << ' ' << c; });
  std::cout << '\n';
  for (char c : rawStringRange) {
    std::cout << c << ' ';
  }
  for (char c : std::ranges::subrange{rawString, NullTerm{}}) {
    std::cout << c << ' ';
  }

  return 0;
}
