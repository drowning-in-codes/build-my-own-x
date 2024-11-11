// #include <cerrno>
#include <cassert>
#include <cerrno>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <variant>
enum LoginErrc { success = 0, not_valid_pass, not_login };
void func(int c) {
  if (c > std::numeric_limits<char>::max()) {
    throw std::invalid_argument("Invalid argument");
  }
}
void term_func() {
  std::cerr << "terminate handler called\n";
  std::abort();
}
constexpr int foo(int i) { return i * i; }
std::optional<int> bar(int i) {
  if (i < 0) {
    return std::nullopt;
  }
  return i * i;
}
// optional 可用作可能失败的工厂的返回类型
std::optional<std::string> create(bool b) {
  if (b)
    return "Godzilla";
  return {};
}
auto parse_num(std::string_view &str) -> std::expected<int, std::string> {
  if (str.empty()) {
    return std::unexpected<std::string>("empty string");
  }
  size_t pos;
  int i = std::stoi(std::string(str), &pos);
  str.remove_prefix(pos);
  return i;
}
int main() {
  std::variant<int, float> v, w;
  v = 132;
  int i = std::get<int>(v);
  assert(42 == i);
  std::tuple<int> tint{1, 2};

  auto error_code = std::make_error_code(std::errc::invalid_argument);
  std::cout << error_code.message() << std::endl;
  std::cout << error_code.value() << std::endl;
  std::cout << error_code.category().name() << std::endl;
  std::optional<int> oi{};
  std::optional<std::string> ostr{std::in_place, "Godzilla"};
  if (oi == std::nullopt) {
    std::puts("oi is nullopt");
  }
  std::set_terminate(term_func);
  setlocale(LC_ALL, "en_US.utf8");
  std::puts("Hello, World!");
  std::error_code ec;
  FILE *fp;
  fp = fopen("file.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Value of errno: %d\n", errno);
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    perror("Error printed by perror");
  } else {
    fclose(fp);
  }
  try {
    func(3);
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
