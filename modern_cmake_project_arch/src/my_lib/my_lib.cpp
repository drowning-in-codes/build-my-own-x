#include "my_lib.h"
#include <iostream>

/**
 * @file 文件名
 * @brief 简介
 * @details 细节
 * @mainpage 工程概览
 * @author 作者
 * @email 邮箱
 * @version 版本号
 * @date 年-月-日
 * @license 版权
 */

/**
 * @brief 函数简介
 *
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 *   @retval 返回值说明
 */
void sayHi() { std::cout << "Hi!" << std::endl; }

int getNumbers() { return 42; }

/**
 * @brief Adds two integers and returns the sum.
 * @param a First integer.
 * @param b Second integer.
 * @return Sum of a and b.
 */
int add(int a, int b) { return a + b; }

void print_hello_world() { std::cout << "Hello World!" << std::endl; }

std::uint32_t factorial(std::uint32_t number) {
  return number <= 1 ? number : factorial(number - 1) * number;
}
