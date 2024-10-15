#include <condition_variable>
#include <cstdio>
#include <exception>
#include <future>
#include <iostream>
#include <mutex>
#include <new>
#include <queue>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;
std::mutex cv_mutex{};
std::condition_variable cv{};
bool flag = false;

void wait_for_flag() {
  std::unique_lock ul{cv_mutex};
  // ul.unlock(); and block the thread
  cv.wait(ul, [] { return flag; });
  // when get notified,relive the thread and lock the mutex
  std::cout << "arrived\n";
}

void set_flag_true() {
  {
    std::lock_guard lg{cv_mutex};
    std::cout << "set_flag_true\n";
    std::this_thread::sleep_for(1s);
    flag = true;
  }
  cv.notify_all();
}
class AudioPlayer {
public:
  AudioPlayer() : stop{false}, player_thread{&AudioPlayer::playMusic, this} {}
  ~AudioPlayer() {
    while (!audio_queue.empty()) {
      std::this_thread::sleep_for(50ms);
    }
    stop = true;
    cond.notify_all();
    if (player_thread.joinable()) {
      player_thread.join();
    }
  }
  void addAudioPath(const std::string &path) {
    std::lock_guard lock{mtx};
    audio_queue.push(path);
    cond.notify_one();
  }

private:
  void playMusic() {
    while (!stop) {
      std::string path;
      {
        std::unique_lock lock{mtx};
        cond.wait(lock, [this] { return !audio_queue.empty() || stop; });
        if (audio_queue.empty())
          return;
        path = audio_queue.front();
        audio_queue.pop();
      }

      // if (!music.openFromFile(path)) {
      //   std::cerr << "无法加载音频文件: " << path << std::endl;
      //   continue;
      // }
      // music.play();
      // while (music.getStatus() == sf::Music::Playing) {
      //   // std::this_thread::sleep_for(50ms);
      //   sf::sleep(sf::seconds(0.1f));
      // }
    }
  }

  std::atomic<bool> stop;
  std::thread player_thread;
  std::mutex mtx;
  std::condition_variable cond;
  std::queue<std::string> audio_queue;
  // sf::Music music;
};
template <typename T> class threadsafe_queue {
  mutable std::mutex m;
  std::condition_variable data_cond;
  std::queue<T> data_queue;

public:
  threadsafe_queue() {};
  void push(T new_value) {
    {
      std::lock_guard lk{m};
      data_queue.push(new_value);
    }
    data_cond.notify_one();
  }
  void pop(T &value) {
    std::unique_lock lk{m};
    // unlock the mtx and block the thread
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    value = data_queue.front();
    data_queue.pop();
  }
  // 从队列中弹出元素,阻塞直到队列不为空, 返回指向弹出元素的指针
  std::shared_ptr<T> pop() {
    std::unique_lock lk{m};
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
    data_queue.pop();
    return res;
  }
  bool empty() const {
    std::lock_guard lk{m};
    return data_queue.empty();
  }
};
int task(int n) {
  std::cout << "异步任务 ID: " << std::this_thread::get_id() << '\n';
  return n * n;
}
std::condition_variable cvb;
std::mutex mtx;
bool ready{false};
int data{};
void process() {
  {
    std::lock_guard lg{mtx};
    // do something
    std::this_thread::sleep_for(100ms);
    data = 1;
    ready = true;
    std::cout << "??\n";
  }
  cvb.notify_one();
}
void get_data() {
  std::unique_lock ul{mtx};
  std::cout << "wait...\n";
  cvb.wait(ul, [] { return ready; });
  std::cout << "get_data" << data << '\n';
}
void f() { std::cout << "f: " << std::this_thread::get_id() << '\n'; }
struct X {
  int operator()(int &n) {
    std::this_thread::sleep_for(3s);

    return n;
  }
  // constexpr explicit operator int() const { return 10; } // 转换方法
  int operator()(int &&n) { return n; }
};
struct Y {
  int f(int n) const { return n * n; }
};
class Test {
public:
  static void sayHi() { std::cout << "Hi\n"; }
};
// class Object {
// public:
//   explicit operator bool() const { return true; }
// };
void crack(int, int) {};
void boo(int &n) {}

int foo(int n) { return 2 * n; }
void calculate_square(std::promise<int> promiseObj, int num) {
  std::this_thread::sleep_for(100ms);
  if (!num) {
    promiseObj.set_value(num * num);
  } else {
    promiseObj.set_exception(std::current_exception());
  }
}
void set_val(int &n) { n = 20; }

std::string fetch_data() {
  std::this_thread::sleep_for(100ms);
  return "从网络获取的数据";
}

int main() {
  // if (Object{}) {
  //   std::cout << "true\n";
  // }
  //
  // auto ck = std::bind(crack, std::placeholders::_1, 1);
  // ck(1);
  std::future<std::string> future_data = std::async(std::launch::async,fetch_data);

  std::shared_future<std::string> shared_future_data = future_data.share();
  std::thread thread1([shared_future_data]{
    std::cout<<"线程1: 等待数据中...\n";
    shared_future_data.wait();
    std::cout<<"线程1: 收到数据..."<<shared_future_data.get()<<std::endl;
  });
  std::thread thread2([shared_future_data]{
    std::cout<<"线程2: 等待数据中...\n";
    shared_future_data.wait();
    std::cout<<"线程2: 收到数据..."<<shared_future_data.get()<<std::endl;
  });
  thread1.join();
  thread2.join();
  int tn{0};
  std::jthread jd{set_val, std::ref(tn)};
  jd.join();
  std::cout << tn << '\n';
  std::promise<int> promise;
  std::future<int> fut_3 = promise.get_future();
  int num{5};
  std::thread t1t(calculate_square, std::move(promise), num);
  try {
    std::cout << "等待线程执行...\n";
    int result = fut_3.get();
    std::cout << "Result is " << result << std::endl;
  } catch (std::exception &e) {
    std::cerr << "来自线程的异常" << e.what() << '\n';
  }
  std::thread{[]{std::puts("Hi");}}.join();

  t1t.join();
  auto test_1 = std::async(&Test::sayHi);
  test_1.wait();
  auto task_2{std::packaged_task<int(int)>{foo}};
  // std::packaged_task<int(int)> task_2{f};
  auto fut_2{task_2.get_future()};
  std::thread fut_t{std::move(task_2), 10};
  // task_2(10);
  fut_t.join();
  std::cout << fut_2.get() << '\n';
  std::packaged_task<double(int, int)> task1(
      [](int a, int b) { return a / b; });
  // task1(10, 2); // 无法获得值 有默认的()重载返回值是 void
  auto fut = task1.get_future();
  task1(10, 2);
  std::cout << fut.get() << '\n';
  Y y;
  int n{10};
  auto t1a = std::async(X{}, 10);
  auto t2a = std::async(&Y::f, &y, 10);
  auto t3a = std::async([] {});
  auto t4a = std::async(X{}, std::ref(n));
  std::cout << t4a.get();
  t3a.wait();

  std::jthread t1{wait_for_flag}, t2{wait_for_flag}, t3{set_flag_true};
  std::jthread jt{get_data}, jt2{process};
  jt.join();
  jt2.join();
  auto f1 = std::async(std::launch::deferred, f);
  f1.wait();
  auto f2 = std::async(std::launch::async, f);
  auto f3 = std::async(std::launch::deferred | std::launch::async, f);
  std::future<int> future = std::async(task, 10);
  std::cout << "main: " << std::this_thread::get_id() << '\n';
  std::cout << future.get() << '\n';
  std::cout << std::boolalpha << future.valid() << '\n';
  return 0;
}
