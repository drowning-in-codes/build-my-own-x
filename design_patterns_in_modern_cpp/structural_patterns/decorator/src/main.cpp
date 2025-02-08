#include <iostream>

class Component {
 public:
  virtual ~Component() = default;
  virtual std::string Operation() const = 0;
};

class ConcreteComponent : public Component {
 public:
  std::string Operation() const override { return "ConcreteComponent"; }
};

class Decorator : public Component {
 protected:
  Component* component_;

 public:
  explicit Decorator(Component* component) : component_(component) {}
};

class ConcreteDecoratorA : public Decorator {
 public:
  explicit ConcreteDecoratorA(Component* component) : Decorator(component) {}

  std::string Operation() const override {
    return "ConcreteComponentA(" + Decorator::Operation() + ")";
  }
};

class ConcreteDecoratorB : public Decorator {
 public:
  explicit ConcreteDecoratorB(Component* component) : Decorator(component) {}

  std::string Operation() const override {
    return "ConcreteComponentB(" + Decorator::Operation() + ")";
  }
};

int main() {
  Component* simple = new ConcreteComponent;
  simple->Operation();

  auto* decorator1 = new ConcreteDecoratorA(simple);
  decorator1->Operation();

  auto* decorator2 = new ConcreteDecoratorB(decorator1);
  decorator2->Operation();
  delete simple;
  delete decorator1;
  delete decorator2;

  return 0;
}
