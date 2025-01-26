#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>
// 有一些关键的范围适配器和工厂，可以很容易地创建视图，或将范围转换为具有特定特征
// (独 立于内容) 的视图: • std::views::all()
// 是将传递的范围转换为视图的主要范围适配器。 • std::views::counted() 可将传入的
// begin 和 count/size 转换为视图的主要范围工厂。 • std::views::common()
// 是一个范围适配器，将 (开始) 迭代器和哨兵 (结束迭代器) 具有不同类型
// 的范围转换为具有统一的开始和结束类型的视图。
std::vector<int> getColl() { return {1, 2, 3}; }
int main() {
  std::vector coll{1, 2, 3};
  std::ranges::iota_view aView{1};
  // 已经是视图 拷贝
  [[maybe_unused]]
  auto v1 = std::views::all(aView);
  std::cout << typeid(v1).name() << std::endl;
  [[maybe_unused]]
  auto v2 = std::views::all(coll);
  // std::cout << typeid(v2).name() << std::endl;
  // 左值
  [[maybe_unused]]
  auto v3 = std::views::all(std::views::all(coll));
  // 右值
  auto v4 = std::views::all(getColl());
  auto v5 = std::views::all(std::move(coll));

  // 类型 std::views::all_t<> 作为 all()
  // 产生的类型，其遵循完全转发规则，若依值类
  // 型和右值引用类型都可以用来指定右值的类型
  std::vector coll_v2{1, 2, 3};
  [[maybe_unused]]
  // std::views::all_t<decltype(coll_v2)> a1{coll_v2};
  std::views::all_t<decltype(coll_v2)> a4{std::move(coll_v2)};
  // std::views::all_t<decltype(coll_v2) &> a5{std::move(coll_v2)};
  std::views::all_t<decltype(coll_v2) &&> a6{std::move(coll_v2)};
  [[maybe_unused]]
  std::views::all_t<decltype(coll_v2) &> a2{coll_v2};
  // std::views::all_t<decltype(coll_v2) &&> a3{coll_v2};
  std::list lst{1, 2, 3, 4};
  auto c = std::views::counted(lst.begin(), 5);
  lst.insert(++lst.begin(), 0);

  std::list lst_2{1, 2, 3};
  auto c2 = std::views::drop(lst_2, 2) | std::views::take(1);

  auto vec5 = std::ranges::find(coll_v2, 5);
  [[maybe_unused]]
  auto v6 = std::views::counted(vec5, 3);
  std::deque<int> deq{1, 2, 3};
  auto deq2 = std::ranges::find(deq, 5);
  std::ranges::iota_view iv{1, 10};
  [[maybe_unused]]
  auto vv1 = std::views::common(lst);
  [[maybe_unused]]
  auto vv2 = std::views::common(iv);
  [[maybe_unused]]
  auto vv3 = std::views::common(std::views::all(coll));
  auto vt = std::views::take(lst, 5);
  [[maybe_unused]]
  auto vv4 = std::views::common(vt);

  std::vector vec{1, 2, 3};
  auto pos1 = vec.begin();
  for (std::counted_iterator pos{coll.begin(), 5}; pos.count() > 0; ++pos) {
    std::cout << *pos << '\n';
  }
  std::cout << '\n';
  for (std::counted_iterator pos{coll.begin(), 5}; pos != std::default_sentinel;
       ++pos) {
    std::cout << *pos << '\n';
  }
}
