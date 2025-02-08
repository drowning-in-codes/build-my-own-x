#include <iostream>
#include <utility>

class Command {
 public:
  virtual ~Command() = default;
  virtual void Execute() const = 0;
};

class SimpleCommand : public Command {
 private:
  std::string pay_load_;

 public:
  explicit SimpleCommand(std::string pay_load)
      : pay_load_(std::move(pay_load)) {}

  void Execute() const override {
    std::cout << "SimpleCommand: See, I can do simple things like printing ("
              << pay_load_ << ")\n";
  }
};

class Receiver {
 public:
  void DoSomething(std::string a) {
    std::cout << "Receiver: Working on (" << a << ")\n";
  }

  void DoSomethingElse(std::string b) {
    std::cout << "Receiver: Also working on (" << b << ")\n";
  }
};

class ComplexCommand : public Command {
 private:
  Receiver* receiver_;
  std::string a_;
  std::string b_;

 public:
  ComplexCommand(Receiver* receiver, std::string a, std::string b)
      : receiver_(receiver), a_(std::move(a)), b_(std::move(b)) {}

  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver "
                 "object.\n";
    this->receiver_->DoSomething(this->a_);
    this->receiver_->DoSomethingElse(this->b_);
  }
};

class Invoker {
 private:
  Command* on_start_;
  Command* on_finish_;

 public:
  ~Invoker() {
    delete on_start_;
    delete on_finish_;
  }

  void SetOnStart(Command* command) { on_start_ = command; }

  void SetOnFinish(Command* command) { on_finish_ = command; }

  void DoSomethingImportant() {
    std::cout << "Invoker: Does anybody want something done before I begin?\n";
    if (this->on_start_) {
      on_start_->Execute();
    }
    std::cout << "Invoker: ...doing something really important...\n";
    if (on_finish_) {
      on_finish_->Execute();
    }
  }
};

int main() {
  auto* invoker = new Invoker;
  invoker->SetOnStart(new SimpleCommand("Say Hi!"));
  auto* receiver = new Receiver;
  invoker->SetOnStart(
      new ComplexCommand(receiver, "Send email", "Save report"));
  invoker->DoSomethingImportant();
  delete invoker;
  delete receiver;
  return 0;
}
