#include <iostream>
class BaseComponent;

class Mediator {
 public:
  virtual void Notify(BaseComponent* sender, std::string event) const = 0;
  virtual ~Mediator() = default;
};

class BaseComponent {
 protected:
  Mediator* mediator_;

 public:
  explicit BaseComponent(Mediator* mediator = nullptr) : mediator_{mediator} {};

  void set_mediator(Mediator* mediator) { mediator_ = mediator; }
};

class Component : public BaseComponent {
 public:
  using BaseComponent::BaseComponent;

  void DoA() {
    std::cout << "Component A does A.\n";
    mediator_->Notify(this, "A");
  }

  void DoB() {
    std::cout << "Component A does B.\n";
    mediator_->Notify(this, "B");
  }
};

class Component2 : public BaseComponent {
 public:
  void DoC() {
    std::cout << "Component B does C.\n";
    this->mediator_->Notify(this, "C");
  }
};

class ConcreteMediator : public Mediator {
 private:
  Component* component1_;
  Component2* component2_;

 public:
  ConcreteMediator(Component* component1, Component2* component2)
      : component1_{component1}, component2_{component2} {
    component1_->set_mediator(this);
    component2_->set_mediator(this);
  }

  void Notify(BaseComponent* sender, std::string event) const override {
    if (event == "A") {
      std::cout << "Mediator reacts on A and triggers following operations:\n";
      this->component1_->DoA();
    }
    if (event == "D") {
      std::cout << "Mediator reacts on D and triggers following operations:\n";
      this->component1_->DoB();
      this->component2_->DoC();
    }
  }
};

int main() {
  auto* c1 = new Component;
  auto* c2 = new Component2;
  ConcreteMediator mediator{c1, c2};
  std::cout << "Client triggers operation A.\n";
  c1->DoA();
  std::cout << "\n";
  std::cout << "Client triggers operation D.\n";
  c2->DoC();

  delete c1;
  delete c2;
}
