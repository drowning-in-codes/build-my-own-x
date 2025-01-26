#include <iostream>
#include <list>
#include <ranges>
#include <vector>

namespace vws = std::views;
// auto getValues() {
//   std::vector<int> coll{1, 2, 3, 4, 5};
//   //
//   这段代码与返回指向局部对象的引用或指针一样糟糕，可能会正常工作或导致致命的运行时错
//   // 误。不幸的是，编译器 (目前) 不会对此发出警告
//   return coll | std::views::drop(2);
// }
// •若 coll 已经是一个视图，take() 会按原样获取视图。
// •若 coll 是一个容器，take() 使用一个到容器的视图，该视图是用适配器
// std::views::all() 自动创 建的。适配器会产生一个 ref_view，若容器通过名称传递
// (作为左值)，将引用容器的所有元素。 •若传递了一个右值
// (临时范围，比如函数返回的容器或带有 std::move() 标记的容器)，则该范
// 围将移动到 owning_view 中，然后 owning_view
// 直接保存传递类型的范围，其中包含所有移 动的元素。[C++20
// 发布http://wg21.link/p2415r2后增加了对视图临时对象 (右值) 的 支持
// const&并不适用于所有视图

void print(const auto &rg) {
  for (const auto &elem : rg) {
    std::cout << elem << " ";
  }
  std::cout << '\n';
}

auto getValues() {
  std::vector coll{1, 2, 3, 4, 5};
  return std::move(coll) | vws::drop(2);
}
int main() {
  std::vector vec{1, 2, 3, 4};
  std::list lst{1, 2, 3};
  print(vec | std::views::take(1));
  print(lst | std::views::take(1));
  print(vec | std::views::drop(1));
  // print(lst | std::views::drop(1));
  for (const auto &elem : lst | std::views::drop(3)) {
    std::cout << elem << " ";
  }
  [[maybe_unused]]
  auto isEven = [](const auto &val) { return !(val % 2); };
  // print(vec | std::views::filter(isEven));
  auto values = getValues();
  std::vector<int> coll{8, 15, 7, 0, 9};
  // 视图不应该修改传递的参数或为其调用非 const 操作
  //  coll | vws::transform([](auto &val) {
  //   val += 10; // undefined behavior
  //   return val;
  // });
  // 惰性计算
  auto vColl = coll | vws::filter([](int i) { return i % 3 == 0; }) |
               vws::transform([](int i) { return i * i; });
  // ，定义视图 vColl 时或之后，不会调用 filter() 和
  // transform()。当使用视图时，处理就开始了 (这里: 迭代
  // vColl)。视图使用延迟求值
  for (int val : vColl) {
    std::cout << val << " ";
  }
  auto pos_v = coll.begin();
  auto pos = vColl.begin();
  std::ranges::subrange rngs{coll.begin(), coll.begin() + 2};
  std::cout << *pos << std::endl;
  // 缓存视图
  // 第二次使用 vcol .begin() 时，不再尝试查找第一个元素，跳过的元素视图会在调用
  // begin() 后进行缓存

  return 0;
}
