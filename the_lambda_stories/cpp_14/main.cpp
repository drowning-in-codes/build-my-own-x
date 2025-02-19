#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
using namespace std::string_literals;
auto func() /*-> int*/ {
  int x{};
  return x;
}
auto createMulLambda(int x) noexcept {
  // struct {
  // private:
  //   int x;
  //
  // public:
  //   int operator()(int param) { return x * param; }
  // } __instance;
  return [x](int param = 0) noexcept { return x * param; };
}
struct Baz {
  auto foo() const {
    return [s = s]() { std::cout << s << '\n'; };
  }
  std::string s;
};
template <typename T> auto sum(T num) { return num; }

template <typename T, typename... Args> auto sum(T num, Args... args) {
  return num + sum(args...);
}
void fooz(const std::string &) { std::cout << "foo(const std::string&)"; }
void fooz(std::string &&) { std::cout << "foo(const std::string&)"; }
void bazz(int i) { std::cout << "int: " << i << '\n'; }
void bazz(float i) { std::cout << "float: " << i << '\n'; };
int main(int argc, char *argv[]) {
  // recursive lambda
  const std::function<int(int)> fact = [&fact](int n) {
    return n > 1 ? n * fact(n - 1) : 1;
  };
  const auto factorial = [](int n) noexcept {
    const auto f_impl = [](int n, const auto &impl) noexcept -> int {
      return n > 1 ? n * impl(n - 1, impl) : 1;
    };
    return f_impl(n, f_impl);
  };

  const std::vector<int> vi{1, 2, 3, 4};
  std::for_each(vi.begin(), vi.end(),
                [](auto &&x) { bazz(std::forward<decltype(x)>(x)); });
  const std::map<std::string, int> numbers{{"one", 1}};
  std::for_each(numbers.begin(), numbers.end(),
                [](const auto /*const std::pair<std::string, int>*/ &entry) {
                  std::cout << entry.first;
                });
  const auto perfect_forwarding_lambda = [](auto &&arg) {
    fooz(std::forward<decltype(arg)>(arg));
  };
  const auto sumLambda = [](auto... args) {
    std::cout << sizeof...(args);
    return sum(args...);
  };
  const auto f1 = Baz{"abc"}.foo();
  const auto lam = [](int x = 10) { std::cout << x << '\n'; };
  const auto fo = [](auto x, int y) { std::cout << x << ", " << y << '\n'; };
  lam();
  lam(100);
  int x{30};
  int y{50};
  const auto lambda = createMulLambda(10);
  const auto foo = [z = x + y]() { std::cout << z << '\n'; };
  const auto bar = [&z = x]() {
    std::cout << z << '\n';
    z = 10;
  };
  bar();
  x = 0;
  bar();
  std::unique_ptr<int> p{new int(10)};
  const auto baz = [ptr = std::move(p)]() {
    std::cout << "pointer in lambda" << ptr.get();
  };
  std::cout << "pointer in main()" << p.get() << '\n';
  baz();
  const auto prefix = "ada";
  const std::vector<std::string> vs = {"apple", "orange", "foobar"};
  auto result =
      std::find_if(vs.begin(), vs.end(), [&prefix](const std::string &s) {
        return s == prefix + "bar"s;
      });
  if (result != vs.end()) {
    std::cout << prefix << "-something found! \n";
  }
  result = std::find_if(vs.begin(), vs.end(),
                        [savedString = prefix + "bar"s](const std::string &s) {
                          return s == savedString;
                        });
  if (result != vs.end()) {
    std::cout << prefix << "-something found! \n";
  }

  return 0;
}
