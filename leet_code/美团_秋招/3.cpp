#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  int n, x, y;
  std::cin >> n >> x >> y;
  std::vector<int> values(n);
  for (int i{}; i < n; i++) {
    std::cin >> values[i];
  }
  std::sort(values.begin(), values.end());
  for (int faild_p = x; faild_p <=y; faild_p++) {
    int grade = values.at(faild_p-1);
    //大于淘汰分数 小于晋级分数
    //晋级人数
    int passed_p = n - faild_p;
    if(passed_p>=x && passed_p<=y) {
        std::cout<<grade;
        return 0;
    }
  }

  std::cout << -1 << std::endl;

  return 0;
}