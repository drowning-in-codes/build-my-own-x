#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <ranges>
#include <semaphore>
#include <syncstream>
#include <thread>
template <typename T> class has_size {
  template <typename U> static std::true_type test(decltype(&U::size));

  template <typename U> static std::false_type test(...);

public:
  static constexpr bool value = decltype(test<T>(0))::value;
};
using namespace std::chrono_literals;
inline auto syncOut(std::ostream &strm = std::cout) {
  return std::osyncstream{strm};
}
void squareRoots(int num) {
  for (int i : std::views::iota(0, num)) {
    std::osyncstream outputSync{std::cout};
    outputSync << "i=" << i << std::endl << std::flush_emit;
  }
}
void squareRoot(std::ostream &strm, int num) {
  std::osyncstream syncStrm{strm};
  for (int i : std::views::iota(0, num)) {
    syncStrm << "i=" << i << std::endl << std::flush_emit;
  }
}
struct A {
  std::size_t size() const { return 10; }
};
struct B {};
int main() {
  std::cout << "A has size: " << has_size<A>::value << '\n';
  std::cout << "B has size: " << has_size<B>::value << '\n';
  std::jthread t1{squareRoots, 10};
  std::jthread t2{squareRoots, 20};
  std::ofstream fs{"tmp.out"};
  std::jthread t3{squareRoot, std::ref(fs), 10};

  std::queue<char> values;
  std::mutex valuesMx;
  int sharedData = 0;
  std::binary_semaphore dataReady{0};
  std::binary_semaphore dataDone{0};
  std::jthread process{[&](std::stop_token st) {
    while (!st.stop_requested()) {
      if (dataReady.try_acquire_for(1s)) {
        int data = sharedData;
        std::cout << "[process] read " << data << std::endl;
        std::this_thread::sleep_for(data * .5s);

        dataDone.release();
      } else {
        std::cout << "[process] no data to process\n";
      }
    }
  }};
  for (int i = 9; i < 10; ++i) {
    std::cout << "[main] write " << i << std::endl;
    sharedData = i;
    dataReady.release();
    dataDone.acquire();
    std::cout << "[main] data processed\n";
  }

  for (int i = 0; i < 1000; ++i) {
    values.push(static_cast<char>('a' + (i % ('z' - 'a'))));
  }
  constexpr int numThreads = 10;
  std::counting_semaphore<numThreads> enabled{0};
  std::vector<std::jthread> pool;
  for (int idx = 0; idx < numThreads; ++idx) {
    std::jthread t{[idx, &enabled, &values, &valuesMx](std::stop_token st) {
      while (!st.stop_requested()) {
        enabled.acquire();
        char val;
        {
          std::lock_guard lg{valuesMx};
          val = values.front();
          values.pop();
        }
        for (int i = 0; i < 10; i++) {
          std::cout.put(val);
          auto dur = 130ms * ((idx % 3) + 1);
          std::this_thread::sleep_for(dur);
        }
        enabled.release();
      }
    }};
    pool.push_back(std::move(t));
  }
  std::cout << "====================" << std::endl;
  std::this_thread::sleep_for(2s);

  std::cout << "====================" << std::endl;
  enabled.release(3);
  std::this_thread::sleep_for(2s);

  for (auto &t : pool) {
    t.request_stop();
  }
}
