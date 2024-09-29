#include <Awaiter.hpp>
#include <coroutine>
#include <string>

class [[nodiscard]] CoroTask {
public:
  struct promise_type {
    int coroValue = 0;
    int result = 0;
    std::string backValue{};
    // init and return coroutine interface
    // 创建协程接口 创建对象
    auto get_return_object() { return CoroTask{CoroHdl::from_promise(*this)}; }
    // 主动启动还是需要resume
    auto initial_suspend() { return std::suspend_always{}; }
    // 到达结束时的操作
    void return_void() {}
    // 使用co_yield返回值
    auto yield_value(int val) {
      coroValue = val;
      backValue.clear();
      // return std::suspend_always{};
      return BackAwaiter<CoroHdl>{};
    }
    // 使用co_return返回值
    //  void return_value(const int value) { result = value; }
    //  异常处理
    void unhandled_exception() {}
    // 最终是否挂起
    auto final_suspend() noexcept { return std::suspend_always{}; }
  };
  using CoroHdl = std::coroutine_handle<promise_type>;

private:
  CoroHdl hdl;

public:
  CoroTask(auto h) : hdl{h} {}
  ~CoroTask() {
    if (hdl)
      hdl.destroy();
  }
  CoroTask(const CoroTask &) = delete;
  CoroTask &operator=(const CoroTask &) = delete;

  bool resume() const {
    if (!hdl || hdl.done())
      return false;
    hdl.resume();
    return !hdl.done();
  }
  int getValue() const { return hdl.promise().coroValue; }
  int getResult() const { return hdl.promise().result; }
  void setBackValue(const auto &val) { hdl.promise().backValue = val; }
};
