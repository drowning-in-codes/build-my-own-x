#include <iostream>
#include <templates.hpp>
// 概念是一个模板，应用于传递的模板参数的一个或多个需求引入名称，以便可以将这些需求用
// 作约束。
int main() {
  std::cout << maxValue(3, 1);
  return 0;
}
