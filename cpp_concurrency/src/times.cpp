#include <barrier>
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <future>
#include <iostream>
#include <latch>
#include <numeric>
#include <random>
#include <ranges>
#include <semaphore>
#include <syncstream>
#include <thread>
using namespace std::chrono_literals;
std::mutex m{};
bool done{};
std::condition_variable cv{};
bool wait_loop() {
  const auto timeout = std::chrono::steady_clock::now() + 500ms;
  std::unique_lock lk{m};
  while (!done) {
    if (cv.wait_until(lk, timeout) == std::cv_status::timeout) {
      std::cout << "超时 500ms\n";
      return false;
    }
  }
  return true;
}
std::binary_semaphore smph_signal_main_to_thread{0};
std::binary_semaphore smph_signal_thread_to_main{0};
std::counting_semaphore<3> semaphore{3};

void handle_request(int request_id) {
  std::cout << "进入handle_request尝试获取信号量\n";
  semaphore.acquire(); // similar to cv.wait(lk)??
  std::cout << "成功获取信号量\n";
  // do something
  std::this_thread::sleep_for(1s);

  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution dis{1, 10};
  int process_time = dis(gen);
  std::this_thread::sleep_for(std::chrono::seconds{process_time});
  std::cout << std::format("请求 {} 已被处理\n", request_id);
  semaphore.release();
}
void thread_proc() {
  smph_signal_main_to_thread.acquire();
  std::cout << "[线程]获得信号\n";
  std::this_thread::sleep_for(1s);
  std::cout << "[线程]发送信号\n";
  smph_signal_thread_to_main.release();
}
std::condition_variable cvb;
std::latch work_start{1};
void work() {
  std::cout << "等待其他线程执行\n";
  // work_start.wait();
  work_start.arrive_and_wait();
  std::cout << "任务开始执行\n";
}
std::barrier barrier{3, [n = 1]() noexcept { std::cout << "任务结束\n"; }};
void boo(int n = 2) {}
void f(int start, int end) {
  for (int i = start; i < end; i++) {
    // std::osyncstream{std::cout} << i << '\n';
    std::cout << i << '\n';
    barrier.arrive_and_wait();
    std::this_thread::sleep_for(100ms);
  }
}

int main() {

  std::jthread thread{work};
  std::this_thread::sleep_for(1s);
  // work_start.count_down();
  std::vector<std::jthread> threads;
  for (int i : std::views::iota(0, 9)) {
    threads.emplace_back(f, i, i + 1);
  }
  auto start{std::chrono::system_clock::now()};
  time_t now_time = std::chrono::system_clock::to_time_t(start);
  // std::chrono::steady_clock::time_point
  // end{std::chrono::steady_clock::now()}; std::cout <<
  // std::put_time(std::localtime(&now_time), "%F %T") << std::endl;
  std::chrono::milliseconds ms{3999};

  std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(ms);
  std::cout << s.count() << '\n';

  // template<class Rep,class Period = std::ratio<1>>
  // std::chrono::duration<long,std::ratio<60>> std::ratio<1> == std::ratio<1,1>
  // class duration;

  std::chrono::duration<double> sd =
      std::chrono::duration_cast<std::chrono::duration<double>>(ms);
  auto end = std::chrono::system_clock::now();
  auto result =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::future<int> fut = std::async([] { return 10; });
  std::future_status status;
  if (status = fut.wait_for(10ms); status == std::future_status::ready) {
    std::cout << fut.get() << '\n';
  } else if (status == std::future_status::timeout) {
    std::cout << "等待超时\n";
  } else {
    std::cout << "Lazy Evaluation\n";
  }

  return 0;
}
