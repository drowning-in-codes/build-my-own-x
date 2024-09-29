#include <array>
#include <chrono>
#include <format>
#include <iostream>

void print(const auto &arg1, const auto &arg2) {
  std::cout << std::format("args: {} and {}\n", arg1, arg2);
}
class Always40 {
public:
  int getValue() const { return 40; }
};
enum class Color { red, green, blue };
class Always42 {
public:
  int getValue() const { return 42; }
};
template <> struct std::formatter<Always42> {
  std::formatter<int> f;
  constexpr auto parse(std::format_parse_context &ctx) { return f.parse(ctx); }
  auto format(const Always42 &obj, std::format_context &ctx) const {
    return f.format(obj.getValue(), ctx);
  }
};
//
// template<>
// struct std::formatter<Always42>: std::formatter<int> {
//   auto format(const Always42& obj,std::format_context& ctx)const {
//     return std::formatter<int>::format(obj.getValue(),ctx);
//   }
// };
template <> struct std::formatter<Always40> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
  auto format(const Always40 &obj, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{}", obj.getValue());
  }
};

template <> struct std::formatter<Color> : public std::formatter<std::string> {
  auto format(Color c, format_context &ctx) const {
    std::string value;
    switch (c) {
      using enum Color;
    case red:
      value = "red";
      break;
    case green:
      value = "green";
      break;
    case blue:
      value = "blue";
      break;
    default:
      value = "unknown";
    }
    return std::formatter<std::string>::format(value, ctx);
  }
};
int main() {
  std::string str{"hello"};
  std::cout << std::format("{} world", str) << std::endl;
  std::cout << std::format("{1} is the size of string {0}\n", str, str.size());
  print(std::chrono::system_clock::now(), std::chrono::seconds{13});
  std::cout << std::format("'{0} has value {0:02X} {0:+4d} {0:03o}\n", '?');
  std::cout << std::format("'{0} has value {0:02X} {0:+4d} {0:03o}\n", 'y');
  char buffer[64];
  // 与其他格式化方式相比，std::format()
  // 的实现具有相当好的性能，但必须为结果字符串分配内存。为了节省时间，可以使用
  // std::format_to_n()，将写入预分配的字符数组。还必须指定要写入的
  // 缓冲区及其大小
  auto ret = std::format_to_n(buffer, std::size(buffer) - 1,
                              "String '{}' has {} chars\n", str, str.size());
  *(ret.out) = '\0';
  std::array<char, 5> mem{};
  std::format_to_n(mem.data(), mem.size() - 1, "{}", str);
  std::cout << mem.data() << std::endl;

  // 格式化库还提供了
  // std::format_to()，为格式化的输出写入无限数量的字符。在有限的内存中使
  // 用这个函数有风险，因为如果值需要太多的内存，就会有未定义行为，但通过使用输出流缓冲区迭
  // 代器，也可以安全地使用它写入流

  for (const auto &val : {1, 2, 3}) {
    std::cout << std::format("{}", val);
  }
  std::format_to(std::ostreambuf_iterator<char>{std::cout},
                 "String '{}' has {} chars", str, str.size());

  std::string s;
  std::format_to(std::back_inserter(s), "String '{}' has {} chars\n", str,
                 str.size());
  // 要提前知道格式化输出将写入多少字符 (不写入任何字符)，可以使用
  // std::formatted_size()
  auto sz = std::formatted_size("String '{}' has {} chars\n", str, str.size());
  //  std::format() 应该和 sprintf() 一样快，甚至更好。
  //  std::format_to() 和 std::format_to_n() 应该有更好的性能
  //
  //
  // 应用有时必须在运行时计算格式细节 (例如根据传递的值计算最佳宽度)，必须使用
  // std::vformat() 或 std::vformat_to()，并使用 std::make_format_args()
  // 将所有参数传递给这些函数
  const char *fmt3 = "{:d} {:.2f}\n";
  // std::cout << std::vformat(fmt3, std::make_format_args(42, 1.2));
  std::locale locG("de_DE");
  std::cout << std::format(locG, "Value: {0}\n", 42);
  try {
    const char *fmt4 = "{:s}";
    std::vformat(fmt4, std::make_format_args(42));
  } catch (const std::format_error &e) {
    std::cerr << "Error: " << e.what();
  }
}
