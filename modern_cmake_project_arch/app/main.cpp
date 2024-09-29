#include "config.hpp"
#include <cxxopts.hpp>
#include <iostream>
#include <my_lib.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

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
class Value {
  int value;
};
/**
 * @brief 主函数
 * @details 程序唯一入口
 *
 * @param argc 命令参数个数
 * @param argv 命令参数指针数组
 * @return 程序执行成功与否
 *     @retval 0 程序执行成功
 *     @retval 1 程序执行失败
 * @note 这里只是一个简单的例子
 */
int main() {
  sayHi();
  std::cout << NLOHMANN_JSON_VERSION_MAJOR;
  std::cout << FMT_VERSION;
  std::cout << SPDLOG_VER_MAJOR;
  std::cout << "Hello World!" << std::endl;
  std::cout << project_name;
  return 0;
}
