#include <type_traits>
template <typename T>
  requires(!std::is_pointer_v<T>)
T maxValue(T a, T b) {
  return a > b ? a : b;
}
int main() {
  maxValue(12, 14);
  return 0;
}
