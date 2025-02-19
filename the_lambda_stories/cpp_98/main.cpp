#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

void PrintFunc(int x) { std::cout << x << ' '; }
struct PrintFunctor {
  PrintFunctor(const std::string &str = "", int t_numCalls = 0)
      : strText(std::move(str)), numCalls(t_numCalls) {}
  void operator()(int x) const {
    std::cout << strText;
    std::cout << x << ' ';
    numCalls++;
  }
  std::string strText;
  mutable int numCalls;
};
int main(int argc, char *argv[]) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(1);
  v.push_back(1);
  std::for_each(v.begin(), v.end(), PrintFunc);
  PrintFunctor pf;
  const auto visitor = std::for_each(v.begin(), v.end(), pf);
  std::cout << visitor.numCalls << std::endl;
  const std::size_t samller5 =
      std::count_if(v.begin(), v.end(),
                    std::bind<int>(std::less<int>(), std::placeholders::_1, 5));
  const std::size_t val = std::count_if(
      v.begin(), v.end(),
      std::bind(std::logical_and<bool>(),
                std::bind(std::greater<int>(), std::placeholders::_1, 0),
                std::bind(std::less<int>(), std::placeholders::_1, 5)));
  return 0;
}
