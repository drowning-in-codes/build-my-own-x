// 等号是必需的
// (不能在没有定义的情况下声明一个概念，也不能在这里使用大括号)。等号之后，
// 可以指定可转换为 true 或 false 的编译时表达式。
#include <bit>
#include <concepts>
#include <iostream>
#include <type_traits>
template <typename T>
concept name = requires { typename T::name; };
// 无论在编译时还是在运行时，都可以在需要布尔表达式值的地方使用一个概念。但不接受地址，
// 因为其后面没有对象 (地址是一个纯右值)。
// 模板参数可能没有约束 (不能使用概念来定义概念)。
// 不能在函数中定义概念 (所有模板都是如此)。
//
//
// 概念并不表示代码，没有类型、存储、生命周期或与对象相关的其他属性。
// 通过在编译时为特定模板参数实例化它们，实例化只会变为 true 或
// false，所以可以在使用 true 或 false
// 的地方使用，可以得到这些字面量的所有属性
// 概念不必声明为内联的，其隐式内联。
// 概念可以用作类型约束:
// 概念是给约束命名的唯一方法，所以需要其来决定一个约束是否是另一个约束的特殊情况。
// 包含的概念。为了让编译器决定一个约束是否决定另一个约束
// (因此是特殊的)，必须将约束 公式化为概念。
template <auto Val>
concept LessThan10 = Val < 10;

template <auto Val>
concept PowerOf2 = std::has_single_bit(static_cast<unsigned>(Val));

template <typename T, auto Val>
  requires PowerOf2<Val>
class Memory {};

template <int Val>
  requires LessThan10<Val>
class MyType {};

template <std::convertible_to<int> T> class MyClass {};
auto myFunc([[maybe_unused]] const std::convertible_to<int> auto &val) {

};

// 对于约束可调用对象 (函数、函数对象、Lambda) 的类型，可以使用
// std::invocable 或 std::regular_invocable
// 概念传递一定数量的特定类型的参数，例如：要求传递一个接 受 int 和 std::string
// 的操作
template <std::invocable<int, std::string> Callable> void call(Callable op);

void call(std::invocable<int, std::string> auto op);

// 使用概念作为类型约束
// 类型约束与auto
void foo(const std::integral auto &val) {}
// std::floating_point auto val1 = f();
std::copyable auto foo(auto) {}
template <typename T, std::integral auto Max> class SizedCall {};

// 两个概念可以有一个包含关系，可以指定一个概念，使其限制一个或多个其他概念。这样做的
// 好处是，当两个约束都得到满足时，重载解析更倾向于使用约束较多的泛型代码，而不是使用约束
// 较少的泛型代码
//
//
template <typename T>
concept GeoObject = requires(T obj) {
  { obj.width() } -> std::integral;
  { obj.height() } -> std::integral;
  obj.draw();
};
class Color {};
template <typename T>
concept ColoredGeoObject = GeoObject<T> && requires(T obj) {
  obj.setColor(Color{});
  { obj.getColor() } -> std::convertible_to<Color>;
};
// 约束和概念不会仅仅基于需求进行包含
// 检查假设的工作只针对概念进行评估。若没有使用概念，则具有不同约束的重载不明确:
template <typename T>
  requires std::is_convertible_v<T, int> // type_traits
void print(T) {}
template <typename T>
  requires(std::is_convertible_v<T, int> && sizeof(int) >= 4) // type_traits
void print(T) {}

template <typename T>
  requires std::convertible_to<T, int>
void print(T) {}

template <typename T>
  requires(std::convertible_to<T, int> && sizeof(int) >= 4)
void print(T) {}

template <typename T>
  requires(std::integral<T> && std::is_floating_point_v<T>)
void print(T) {}
// 为了避免混淆，不要对相互包含的概念做太多的假设。若有疑问，请指定所需的所有具体概念
template <typename T>
concept RgSwap = std::ranges::input_range<T> && std::swappable<T>;

template <typename T>
concept ContCopy = std::ranges::contiguous_range<T> && std::copyable<T>;

// 定义概念
// template <typename T, typename U>
// concept SameAs = std::is_same_v<T, U>;
//
template <typename T, typename U>
concept SameAsHelper = std::is_same_v<T, U>;
template <typename T, typename U>
concept SameAs = SameAsHelper<T, U> && SameAsHelper<U, T>;
template <typename T, typename U>
  requires std::same_as<T, U>
void foo(T, U) {}

template <typename T, typename U>
  requires SameAs<T, U>
void foo(T, U) {
  std::cout << "foo(T,U)\n";
}

template <typename T, typename U>
  requires SameAs<U, T> && std::integral<T>
void foo(T, U) {
  std::cout << "foo () for integral parameters of same type\n";
}

// 问题是编译器无法检测到 SameAs<>
// 是可交换的。对于编译器来说，模板参数的顺序很重要，
// 所以第一个需求不一定是第二个需求的子集
int main() { // foo(1, 2);
  [[maybe_unused]] Memory<int, 8> m1;
  // Memory<int, 9> m1;
  // print(43); // ambiguous
  return 0;
}
