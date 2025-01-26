#include <array>
#include <chrono>
#include <condition_variable>
#include <format>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <stop_token>
#include <syncstream>
#include <thread>
using namespace std::literals;
// std::thread 要求在其生命周期结束时，若表示正在运行的线程，则调用
// join()(等待线程结束) 或
// detach()(让线程在后台运行)。若两者都没有调用，析构函数会立即导致异常的程序终止
// (在某些系 统上导致段错误)

int compute(int a, int b) {
  std::this_thread::sleep_for(2s);
  return a + b;
}
int my_task() {
  std::this_thread::sleep_for(1s);
  return 0;
}
auto syncOut(std::ostream &strm = std::cout) { return std::osyncstream{strm}; }
void stop_callback_task(std::stop_token st, int num) {
  auto id = std::this_thread::get_id();
  syncOut() << "call task(" << num << "\n";
  std::stop_callback cb{st, [num, id] {
                          syncOut() << "task(" << num << ") interrupted\n"
                                    << (id == std::this_thread::get_id()
                                            ? "same thread\n"
                                            : "different thread\n");
                        }};
  std::this_thread::sleep_for(1s);
  std::stop_callback cb2{st, [num, id] {
                           syncOut() << "task(" << num << ") interrupted\n";
                           syncOut() << "task(" << num << ") interrupted\n"
                                     << (id == std::this_thread::get_id()
                                             ? "same thread\n"
                                             : "different thread\n");
                         }};
}
void set_value(std::promise<int> prom) { prom.set_value(10); }
int main() {
  std::queue<std::string> messages;
  std::mutex messageMx;
  std::condition_variable_any messageCV;
  std::jthread t1{[&](std::stop_token st) {
    // std::stop_callback sc(st,[](){
    //     std::cout << "stop requested" << std::endl;
    //   });
    while (!st.stop_requested()) {
      std::string msg;
      {
        // unique_lock在构造时只能锁住一个互斥量。但与scoped_lock不同的是，你可以在后续的代码中手动解锁、重新锁住或者在不同的地方重新锁住另一个互斥量
        std::unique_lock lock(messageMx);
        if (!messageCV.wait(lock, st, [&] { return !messages.empty(); })) {
          return;
        }
        msg = messages.front();
        messages.pop();
      }
      std::cout << "msg: " << msg << std::endl;
    }
  }};
  std::future<int> result = std::async(std::launch::async, compute, 10, 20);
  int sum = result.get();
  std::cout << sum << '\n';

  std::packaged_task<int()> task(my_task);
  std::future<int> task_result = task.get_future();
  std::thread(std::move(task)).detach();
  int value = result.get();
  std::cout << "The result is: " << value << std::endl;
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();
  std::thread t(set_value, std::move(prom));
  int value2 = fut.get();
  std::cout << "result" << value2 << std::endl;
  t.join();

  for (std::string s : {"Tic", "Tac"}) {
    std::scoped_lock lg{messageMx};
    // 可以传递多个互斥量给scoped_lock的构造函数，它会自动锁住所有传递的互斥量，并且在scoped_lock的生命周期结束时自动解锁
    messages.push(s);
    messageCV.notify_one();
  }

  std::this_thread::sleep_for(1s);
  std::stop_source ssrc;
  std::stop_token stok{ssrc.get_token()};
  std::stop_callback cb{stok, []() { syncOut() << "stop requested\n"; }};
  auto fut1 = std::async(std::launch::async, [stok] {
    for (int i = 0; i < 10; ++i) {
      stop_callback_task(stok, i);
    }
  });
  ssrc.request_stop();
}
