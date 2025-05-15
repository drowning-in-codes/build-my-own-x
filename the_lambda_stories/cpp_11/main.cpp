#include <algorithm>
#include <cctype>
#include <corecrt_malloc.h>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename Callable> class ComplexFunctor : public Callable {
public:
  explicit ComplexFunctor(Callable f) : Callable(f) {};
};
template <class... Args> void captureTest(Args... args) {
  const auto lamda = [args...] {
    const auto tup = std::make_tuple(args...);
    std::cout << "tuple size: " << std::tuple_size_v<decltype(tup)> << '\n';
    std::cout << "tuple 1st: " << std::get<0>(tup) << '\n';
  };
  lamda();
}
template <typename Callable>
ComplexFunctor<Callable> MakeComplexFunctor(Callable &&cal) {
  return ComplexFunctor<Callable>(cal);
}

template <typename UCall, typename TCall>
class SimpleOverload : public UCall, TCall {
public:
  SimpleOverload(UCall uc, TCall tc) : UCall(uc), TCall(tc) {}
  using TCall::operator();
  using UCall::operator();
};
void callwith10(void (*bar)(int)) { bar(10); }
template <typename F> void calll_func(F f) { f(1); }
int main(int argc, char *argv[]) {
  using TFunc = void (*)(int &);
  std::vector<TFunc> ptrFunc;
  ptrFunc.push_back([](int &x) { std::cout << x << '\n'; });
  ptrFunc.push_back([](int &x) { x *= 2; });
  std::vector<std::function<std::string(std::string &)>> vecFilters;
  std::size_t removedSpaceCounter{};
  const auto removeSpaces = [&removedSpaceCounter](const std::string &str) {
    std::string tmp;
    std::copy_if(str.begin(), str.end(), std::back_inserter(tmp),
                 [](char ch) { return !std::isspace(ch); });
    removedSpaceCounter += str.length() - tmp.length();
    return tmp;
  };
  const auto makeUpperCase = [](const std::string &str) {
    std::string tmp = str;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return tmp;
  };
  vecFilters.emplace_back(removeSpaces);
  vecFilters.emplace_back([](const std::string &x) { return x + "Amazing"; });

  struct {
    using f_ptr = void (*)(int);
    // void operator()(int s) const { return call(s); }
    operator f_ptr() const { return &call; }

  private:
    static void call(int s) { std::cout << s << '\n'; }
  } bar;
  callwith10(bar);
  calll_func(static_cast<int (*)(int)>([](int x) -> int { return 10 + x; }));
  calll_func(static_cast<int (*)(int)>([](int x) -> int { return 10 + x; }));
  []() noexcept { std::cout << "Hi"; }();
  const auto baz = [](int x) noexcept {
    if (x > 20) {
      return x * 1.1;
    } else {
      return x * 2.2;
    }
  };
  static_assert(std::is_same_v<double, decltype(baz(3))>,
                "not the same with double");
  const int val = 10;
  auto foo = [val]() mutable {
    std::cout << std::is_const_v<decltype(val)> << '\n';
    // x++;
  };

  struct {
    void operator()(int x) { std::cout << x << ' '; }
  } instance;
  std::vector<int> v;
  v.push_back(1);
  std::for_each(v.begin(), v.end(), instance);
  int sum = 0;
  std::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
  int x = 1, y = 1;
  [&]() noexcept {
    ++x;
    ++y;
  }(); // <-- call ()
  std::cout << x << ", " << y;
  const auto func = MakeComplexFunctor([]() {});
  return 0;
}
