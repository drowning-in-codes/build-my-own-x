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

int main() {

  return 0;
}