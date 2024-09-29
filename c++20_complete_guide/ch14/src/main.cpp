#include <Awaiter.hpp>
#include <CoroTask.hpp>
#include <coroutine>
#include <iostream>
CoroTask coro(int max) {
  std::cout << "CORO" << max << " start\n";
  for (int val = 1; val <= max; ++val) {
    std::cout << "CORO" << max << " " << val << "\n";
    co_await std::suspend_always{};
    co_await Awaiter{};
    auto back = co_yield val;
    // co_yield val;
    // co_return 1;
  }
}
int main() {
  auto coroTask = coro(3);
  std::cout << "coro() started\n";
  while (coroTask.resume()) {
    std::cout << "coro() suspended";
  }
  std::cout << "coro() done\n";
}
