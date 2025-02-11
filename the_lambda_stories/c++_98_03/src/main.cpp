#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

void PrintFunc(int i) { std::cout << i; }
struct PrintFunctor {
  void operator()(int i) const { std::cout << i; }
};
struct StatePrintFunctor {
  mutable int num;
  StatePrintFunctor() : num(0) {}
  void operator()(int i) const {
    num++;
    std::cout << i + num;
  }
};
int main(int argc, char *argv[]) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  std::for_each(v.begin(), v.end(), PrintFunc);
  std::cout << '\n';
  std::for_each(v.begin(), v.end(), PrintFunctor());
  std::cout << '\n';
  const StatePrintFunctor visitor =
      std::for_each(v.begin(), v.end(), StatePrintFunctor());
  std::cout << visitor.num << '\n';
  // 辅助函数
  const std::size_t smaller5 =
      std::count_if(v.begin(), v.end(),
                    std::bind(std::less<int>(), std::placeholders::_1, 5));

  return 0;
}
