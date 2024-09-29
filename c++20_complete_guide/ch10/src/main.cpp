#include <array>
#include <chrono>
#include <format>
#include <iostream>

namespace chr = std::chrono;
using namespace std::literals;
int main() {
  auto current_time = chr::system_clock::now();
  std::cout << current_time << '\n'; // UTC-8
  chr::year_month_day first = 2021y / 1 / 5;
  for (auto d = first; d.year() == first.year(); d += chr::months{1}) {
    std::cout << d << ":\n";
    auto tp{chr::sys_days{d} + 18h + 30min};
    std::cout << std::format("The time point tp is {:%A %D at %R}\n", tp);
  }
  first = 2021y / 12 / chr::last;
  auto second = 2022y / chr::February / chr::Monday[1];
  auto d1 = std::chrono::years{2024} / 1 / 13;
  std::cout << d1 << '\n';
  for (auto d = second; d.year() == first.year(); d += chr::months{1}) {
    auto tp{chr::sys_days{d} + 18h};
    std::cout << std::format("The time point tp is {:%A %D at %R}\n", tp);
  }
  try {
    auto localNow = chr::current_zone()->to_local(chr::system_clock::now());
    chr::year_month_day today{chr::floor<chr::days>(localNow)};
  } catch (const std::exception &e) {
    std::cerr << "EXCEPTION: " << e.what() << '\n';
  }
  auto tp{chr::local_days{first} + 18h};
  chr::zoned_time timeLocal{chr::current_zone(), current_time};
  chr::zoned_time timeUkraine{"Europe/Kiev", tp};
  std::cout << timeLocal.get_time_zone()->name();
  std::cout << timeLocal.get_local_time() << '\n';
  std::cout << chr::local_days{first} << '\n';
  chr::duration<int> dur{18h};
  std::cout << dur;
}
