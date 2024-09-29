#include <barrier>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>
using namespace std::chrono_literals;

void worker(std::latch &l) {
  std::this_thread::sleep_for(1s);
  l.count_down();
  std::cout << "Worker thread finished and counted down\n";
}
int main() {
  const int num_threads{5};
  std::latch l(num_threads);
  std::vector<std::jthread> threads;
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(worker, std::ref(l));
  }
  l.wait();
  for (auto &t : threads) {
    t.join();
  }

  std::latch allReady{10};
  std::vector<std::jthread> threads2;
  for (int i = 0; i < num_threads; ++i) {
    std::jthread t{[i, &allReady] {
      std::this_thread::sleep_for(2s);
      allReady.arrive_and_wait();
      for (int j = 0; j < i + 5; ++j) {
        std::cout.put(static_cast<char>('A' + j));
        std::this_thread::sleep_for(100ms);
      }
    }};
    threads2.push_back(std::move(t));
  }
  std::vector values{1, 2, 3};
  auto printValue = [&values]() noexcept {
    for (auto val : values) {
      std::cout << val << ' ';
    }
  };
  std::barrier allDone{int(values.size()), printValue};
  std::vector<std::jthread> threads3;
  for (std::size_t idx = 0; idx < values.size(); ++idx) {
    threads3.push_back(std::jthread{[idx, &values, &allDone] {
      for (int i = 0; i < 5; ++i) {
        values[idx] = std::sqrt(values[idx]);
        allDone.arrive_and_wait();
      }
    }});
  }
}
