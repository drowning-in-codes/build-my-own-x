#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <shared_mutex>
#include <string>
#include <thread>
#include <utility>
using namespace std::chrono_literals;
int func(int n) { return n * 2; }
class Task {
  int _num;

 public:
  explicit Task(int num) : _num(num) {};
  int operator()(int n) { return _num * n; }
};

// 向线程传递参数
// 参数会复制到新线程的内存空间中，即使函数中的参数是引用，依然实际是复制
// 因为 std::thread
// 内部会将保有的参数副本转换为右值表达式进行传递，这是为了那些只支持移动的类型，
// 左值引用没办法引用右值表达式
void fl(int n, const int& a) {
  std::cout << &n << std::endl;
  std::cout << &a << std::endl;
}
struct move_only {
  move_only() { std::puts("构造"); }
  move_only(move_only&&) { std::puts("移动构造"); }
  move_only(const move_only&) { std::puts("拷贝构造"); };
};
void ff(move_only) {}
void transfer_para() {
  int num = 10;
  std::cout << &num << std::endl;
  std::jthread jd{fl, 3, std::ref(num)};
}
void loop(std::stop_token st) {
  while (!st.stop_requested()) {
    while (true);
  }
}
struct X {
  // 成员函数
  void task_run(int) const {};
};
class thread_guard {
  std::thread& t;
  explicit thread_guard(std::thread& t_) : t(t_) {}
  ~thread_guard() {
    std::puts("析构");
    if (t.joinable()) {
      t.join();
    }
  }
  thread_guard(const thread_guard&) = delete;
  thread_guard& operator=(const thread_guard&) = delete;
};
void ft(const std::string&) {};
void hi(int& x) { std::cout << "hi" << &x << std::endl; }
template <typename T>
void foo(T&& t) {
  std::cout << typeid(t).name() << std::endl;
  std::cout << t << std::endl;
  std::decay_t<T> num = t;
}
class Task_2 {
 public:
  void operator()() { std::puts("operator()()const\n"); }
  operator bool() { return true; }
};

void h(int&& t) { std::puts("右值引用"); };
void h(int& t) { std::puts("左值引用"); };
int& get_num(int& num) { return num; }
int&& get_num(int&& num) { return static_cast<int&&>(num); }
template <typename T>
void f(T&& n) {
  h(std::forward<T&>(n));
};
void tt(const int&) {};

template <typename T>
void fa(T param) {};
#include <mutex>
std::mutex m{};
bool arrived{false};

void foor() {
  m.lock();
  std::cout << "foor" << std::endl;
  m.unlock();
}

void foor_guard() {
  std::lock_guard<std::mutex> lock{m};
  std::cout << "foor" << std::endl;
}
void add_to_list(int n, std::list<int>& list) {
  std::vector<int> numbers(n + 1);
  std::iota(numbers.begin(), numbers.end(), 0);
  int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
  {
    std::lock_guard<std::mutex> lc{m};
    list.push_back(sum);
  }
}
std::map<std::string, std::string> data_;
std::shared_mutex mtx;
void writeData() {
  std::lock_guard lg{mtx};
  data_["fa"] = "af";
}
void readData() {
  std::shared_lock sl{mtx};
  auto it = data_.find("aa");
  std::cout << (it == data_.end() ? it->second : "");
}
void print_list(const std::list<int>& list) {
  std::lock_guard<std::mutex> lc{m};
  for (auto& i : list) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}
void thread_func(int id) {
  if (m.try_lock()) {
    std::cout << "线程: " << id << "获得锁\n";
    std::this_thread::sleep_for(3s);
    m.unlock();
  } else {
    std::cout << "获取锁失败\n";
  }
}

std::mutex m1, m2;
std::size_t n{};

void f() {
  std::lock_guard<std::mutex> lc1{m1};
  std::lock_guard<std::mutex> lc2{m2};
  ++n;
}

void f2() {
  std::lock_guard<std::mutex> lc1{m1};
  std::lock_guard<std::mutex> lc2{m2};
  ++n;
}

template <typename T>
  requires requires(T mtx) {
    mtx.lock();
    mtx.unlock();
  }
class MyLockGuard {
 public:
  using mutext_type = T;
  explicit MyLockGuard(T t_mutex) : _MyMutex(t_mutex) { _MyMutex.lock(); }
  MyLockGuard(T& _Mtx, std::adopt_lock_t) noexcept : _MyMutex(_Mtx) {}
  MyLockGuard(const MyLockGuard&) = delete;
  MyLockGuard& operator=(const MyLockGuard&) = delete;

  ~MyLockGuard() noexcept { _MyMutex.unlock(); }

 private:
  T& _MyMutex;
};
struct Xa {
  Xa(const std::string& str) : object(str) {}
  friend void swap(Xa& lhs, Xa& rhs);

 private:
  std::string object;
  std::mutex m;
};
void swap(Xa& lhs, Xa& rhs) {
  if (&lhs == &rhs) return;
  std::lock(lhs.m, rhs.m);
  std::lock_guard<std::mutex> lock1{lhs.m, std::adopt_lock};
  std::lock_guard<std::mutex> lock2{rhs.m, std::adopt_lock};
  swap(lhs.object, rhs.object);
}
std::once_flag flag;
void boo() { std::cout << std::this_thread::get_id() << std::endl; }
std::mutex cv_mutex{};
std::condition_variable cv;
void wait_for_arrival() {
  std::unique_lock lck{cv_mutex};
  cv.wait(lck, [] { return arrived; });
  std::cout << "got notified!\n";
}
void simulate_arrival() {
  std::this_thread::sleep_for(1s);
  {
    std::lock_guard lck{cv_mutex};
    arrived = true;
  }
  cv.notify_all();
}
int main() {
  thread_local int thread_local_counter{0};
  std::call_once(flag, [] { std::cout << "Hi\n"; });
  std::list<int> list;
  std::thread t1_list{add_to_list, 3, std::ref(list)};
  std::thread t3_list{print_list, std::ref(list)};
  t1_list.join();
  t3_list.join();
  std::vector<std::thread> threads;
  for (std::size_t i = 0; i < 10; ++i) {
    threads.emplace_back(boo);
  }
  for (auto& thread : threads) {
    thread.join();
  }
  std::cout << std::this_thread::get_id() << std::endl;
  int num2{3};
  auto n2 = std::ref(num2);
  auto t4 = std::decay_t<int&>(n2);
  using _Tuple = std::tuple<std::decay_t<int&>, std::decay_t<int&&>>;
  auto ttc = n2;
  std::cout << ttc << std::endl;
  ttc += 12;
  std::cout << n2 << std::endl;
  std::jthread tta(tt, std::ref(num2));
  const char ch[] = "test";
  fa(ch);
  int num_1 = 10;

  int cccc = std::move(num_1);
  h(get_num(num_1));
  h(get_num(10));
  f(10);
  // 直接创建临时对象即可，可以简化代码并避免引入不必要的局部对象
  Task_2 task_2{};
  std::jthread t{task_2};                     // 临时对象
  std::jthread ta{Task_2{}};                  // 临时对象
  std::jthread ta1{[] { std::puts("Hi"); }};  // 临时对象
  std::jthread ta2{f<int>, num_1};            // 临时对象
  foo(1);
  std::this_thread::sleep_for(1s);
  int num = 0;
  char buffers[1024]{"test"};
  std::jthread ts1(ft, std::string(buffers));
  // std::jthread ts1(ft, "test");
  // 成员函数指针
  // 成员指针必须和对象一起使用，这是唯一标准用法，成员指针不可以转换到函数指针单独使用
  //   即使是非静态成员函数没有使用任何数据成员。
  int x = 20;
  // std::jthread t12(&X::task_run, &x, num);
  // std::jthread tt{std::bind(&X::task_run, &x, std::ref(num))};
  int n = std::jthread::hardware_concurrency();
  std::cout << &"das";  // const char*
  std::cout << std::this_thread::get_id();
  std::jthread t1(func, 2);
  Task task{5};

  std::jthread t2(task, 3);
  move_only obj;
  std::jthread t3(ff, obj);
  // std::jthread t3(loop);
  std::cout << "支持" << n << "个并发线程" << std::endl;
  // std::thread t4(
  //     []() { std::cout << std::this_thread::get_id() << std::endl; });
  // std::thread t5{std::move(t4)};
  // t5.join();
  // if(t4.joinable()) {
  //   t4.join();
  // }
  // 转移所有权
  // std::thread t6;
  // std::thread t7{[] {}};
  // t6 = std::move(t7);
  // t6.join();
  // t7 = std::thread([] {});
  // t7.join();
  transfer_para();
}
