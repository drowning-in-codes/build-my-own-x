#include <iostream>
#include <typeinfo>

class Context;

class State {
 protected:
  Context* context_;

 public:
  virtual ~State() = default;

  void set_context(Context* context) { context_ = context; }

  virtual void Handle1() = 0;
  virtual void Handle2() = 0;
};

class Context {
 private:
  State* state_{};

 public:
  explicit Context(State* state) { this->TransitionTo(state); }

  ~Context() { delete state_; }

  void TransitionTo(State* state) {
    delete this->state_;
    this->state_ = state;
    this->state_->set_context(this);
  }

  void Request1() { this->state_->Handle1(); }

  void Request2() { this->state_->Handle2(); }
};

class ConcreteStateA : public State {
 public:
  void Handle1() override;

  void Handle2() override { std::cout << "ConcreteStateA handles request2.\n"; }
};

class ConcreteStateB : public State {
 public:
  void Handle1() override { std::cout << "ConcreteStateB handles request1.\n"; }

  void Handle2() override {
    std::cout << "ConcreteStateB handles request2.\n";
    this->context_->TransitionTo(new ConcreteStateA);
  }
};

void ConcreteStateA::Handle1() {
  std::cout << "ConcreteStateA handles request1.\n";
  std::cout << "ConcreteStateA wants to change the state of the context.\n";
  this->context_->TransitionTo(new ConcreteStateB);
}

int main() {
  auto* context = new Context(new ConcreteStateA);
  context->Request1();
  context->Request2();
  delete context;
  return 0;
}
