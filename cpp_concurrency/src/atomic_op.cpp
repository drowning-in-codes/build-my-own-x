#include <atomic>
#include <chrono>
#include <iostream>
#include <type_traits>
using std::cout;
namespace chr = std::chrono_literals; // 命名空间别名
// using atomic_int = std::atomic<int>; // 类型别名
class spinlock_mutex {
  std::atomic_flag flag{};

public:
  spinlock_mutex() noexcept = default;
  void lock() noexcept {
    while (flag.test_and_set(std::memory_order_acquire))
      ;
  }
  void unlock() noexcept { flag.clear(std::memory_order_release); }
};
struct Foo {
  void get_value() { cout << "Foo::get()\n"; }
};
Foo array[5]{};
std::atomic<Foo *> p{array};
int main() {
  Foo *xp = p.fetch_add(2);
  xp = (xp -= 1);
  auto foo{std::make_shared<Foo>()};
  foo->get_value();

  cout << "start coding\n";
  if constexpr (std::atomic<int>::is_always_lock_free) {
    cout << "int is always lock free\n";
  }
  std::atomic<bool> b{true};
  bool x = b.load();
  b.store(true);
  x = b.exchange(false);
  auto ref = (b = false);
  std::atomic<int *> aptr{nullptr};
  int *p = new int{10};
  // aptr.load();
  aptr.store(p);
  int *t = aptr.load();
  std::atomic_int aint{0};
  // aint.load();
  aint.store(10);
  std::atomic_flag f{};
  std::cout << ATOMIC_INT_LOCK_FREE << "\n";
}
