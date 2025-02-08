#include <iostream>
#include <vector>

struct Handler {
 public:
  virtual Handler* SetNext(Handler* handler) = 0;
  virtual std::string Handle(std::string request) = 0;
  ~Handler() = default;
};

struct AbstractHandler : Handler {
 private:
  Handler* next_handler_{};

 public:
  AbstractHandler() = default;

  Handler* SetNext(Handler* handler) override {
    next_handler_ = handler;
    return handler;
  }

  std::string Handle(std::string request) override {
    if (next_handler_) {
      return next_handler_->Handle(request);
    }
    return {};
  }
};

/**
 * All Concrete Handlers either handle a request or pass it to the next handler
 * in the chain.
 */
struct MonkeyHanlder : AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "Banana") {
      return "Monkey: I'll eat the " + request;
    }
    return AbstractHandler::Handle(request);
  }
};

struct SquireelHandler : AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "Nut") {
      return "Squireel: I'll eat the " + request;
    }
    return AbstractHandler::Handle(request);
  }
};

struct DogHandler : AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "MeatBall") {
      return "Dog: I'll eat the " + request;
    }
    return AbstractHandler::Handle(request);
  }
};

int main() {
  auto* monkey = new MonkeyHanlder;
  auto* squireel = new SquireelHandler;
  auto* dog = new DogHandler;
  monkey->SetNext(squireel)->SetNext(dog);
  std::vector<std::string> food{"Nut", "Banana", "Cup of coffee"};
  for (const auto& f : food) {
    const std::string result = monkey->Handle(f);
    if (!result.empty()) {
      std::cout << result << std::endl;
    } else {
      std::cout << " " << f << " was left untouched" << std::endl;
    }
  }
  delete monkey;
  delete squireel;
  delete dog;
  return 0;
}
