#include <algorithm>
#include <iostream>
#include <iterator>

class Strategy {
 public:
  virtual ~Strategy() = default;
  virtual std::string do_algorithm(std::string_view) const = 0;
};

class Context {
 private:
  std::unique_ptr<Strategy> strategy_;

 public:
  explicit Context(std::unique_ptr<Strategy>&& strategy = {})
      : strategy_(std::move(strategy)) {}

  void set_strategy(std::unique_ptr<Strategy>&& strategy) {
    strategy_ = std::move(strategy);
  }

  void doSomeBusinessLogic() const {
    if (strategy_) {
      std::cout << "Context: Sorting data using the strategy (not sure how "
                   "it'll do it)\n";
      std::string result = strategy_->do_algorithm("data");
      std::cout << result << '\n';
    } else {
      std::cout
          << "Context: Can't sort data because there is no strategy set\n";
    }
  }
};

class ConcreteStrategyA : public Strategy {
 public:
  std::string do_algorithm(std::string_view data) const override {
    std::string result{data};
    std::sort(std::begin(result), std::end(result));
    return "ConcreteStrategyA: Sorting data using quick sort\n";
  }
};

class ConcreteStrategyB : public Strategy {
 public:
  std::string do_algorithm(std::string_view data) const override {
    std::string result{data};
    std::sort(std::begin(result), std::end(result), std::greater());
    return result;
  }
};

int main() {
  Context context(std::make_unique<ConcreteStrategyA>());
  context.doSomeBusinessLogic();
  std::cout << '\n';
  std::cout << "Client: Strategy is set to reverse sorting.\n";
  context.set_strategy(std::make_unique<ConcreteStrategyB>());
  context.doSomeBusinessLogic();
  return 0;
}
