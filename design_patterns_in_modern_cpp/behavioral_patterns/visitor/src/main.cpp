#include <array>
#include <iostream>
class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
 public:
  virtual void VisitConcreteComponentA(
      const ConcreteComponentA* element) const = 0;
  virtual void VisitConcreteComponentB(
      const ConcreteComponentB* element) const = 0;
};

class Component {
 public:
  virtual ~Component() = default;
  virtual void Accept(Visitor* visitor) const = 0;
};

class ConcreteComponentA : public Component {
 public:
  void Accept(Visitor* visitor) const override {
    visitor->VisitConcreteComponentA(this);
  }

  std::string ExclusiveMethodOfConcreteComponentA() const { return "A"; }
};

class ConcreteComponentB : public Component {

 public:
  void Accept(Visitor* visitor) const override {
    visitor->VisitConcreteComponentB(this);
  }

  std::string SpecialMethodOfConcreteComponentB() const { return "B"; }
};

class ConcreteVisitor1 : public Visitor {
 public:
  void VisitConcreteComponentA(
      const ConcreteComponentA* element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA()
              << " + ConcreteVisitor1\n";
  }

  void VisitConcreteComponentB(
      const ConcreteComponentB* element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB()
              << " + ConcreteVisitor1\n";
  }
};

class ConcreteVisitor2 : public Visitor {
 public:
  void VisitConcreteComponentA(
      const ConcreteComponentA* element) const override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA()
              << " + ConcreteVisitor2\n";
  }

  void VisitConcreteComponentB(
      const ConcreteComponentB* element) const override {
    std::cout << element->SpecialMethodOfConcreteComponentB()
              << " + ConcreteVisitor2\n";
  }
};

int main() {
  std::array<const Component*, 2> components = {new ConcreteComponentA,
                                                new ConcreteComponentB};
  std::cout << "The client code works with all visitors via the base Visitor "
               "interface:\n";
  auto* visitor1 = new ConcreteVisitor1;
  for (const auto* comp : components) {
    comp->Accept(visitor1);
  }
  auto* visitor2 = new ConcreteVisitor2;
  for (const auto* comp : components) {
    comp->Accept(visitor2);
  }
  for (const auto* comp : components) {
    delete comp;
  }
  delete visitor1;
  delete visitor2;
  return 0;
}
