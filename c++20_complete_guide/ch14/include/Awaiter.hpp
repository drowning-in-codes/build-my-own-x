#include <iostream>
class Awaiter {
public:
  bool await_ready() const noexcept {
    std::cout << " await_ready\n";
    return false;
  }
  void await_suspend(auto hdl) const noexcept {
    std::cout << " await_suspend\n";
  }
  void await_resume() const noexcept { std::cout << " await_resume\n"; }
};

template <typename Hdl> class BackAwaiter {
  Hdl hdl = nullptr;

public:
  BackAwaiter() = default;
  bool await_ready() const noexcept {
    std::cout << "BackAwaiter await_ready\n";
    return false;
  }
  void await_suspend(Hdl h) noexcept { hdl = h; }
  auto await_resume() { return hdl.promise().backValue; }
};
