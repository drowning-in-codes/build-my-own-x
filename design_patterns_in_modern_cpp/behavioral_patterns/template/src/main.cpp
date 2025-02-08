#include <iostream>
#include <memory>

class AbstactClass {
 public:
  void TemplateMethod() const {
    BaseOperation1();
    RequiredOperations1();
    BaseOperation2();
    Hook1();
    RequiredOperation2();
    BaseOperation3();
    Hook2();
  }

 protected:
  void BaseOperation1() const {
    std::cout << "AbstractClass says: I am doing the bulk of the work\n";
  }

  void BaseOperation2() const {
    std::cout << "AbstractClass says: But I let subclasses override some "
                 "operations\n";
  }

  void BaseOperation3() const {
    std::cout
        << "AbstractClass says: But I am doing the bulk of the work anyway\n";
  }

  virtual void RequiredOperations1() const = 0;
  virtual void RequiredOperation2() const = 0;
  virtual void Hook1() const {};
  virtual void Hook2() const {};
};

class ConcreteClass1 : public AbstactClass {
 protected:
  void RequiredOperations1() const override {
    std::cout << "ConcreteClass1 says: Implemented Operation1\n";
  }

  void RequiredOperation2() const override {
    std::cout << "ConcreteClass1 says: Implemented Operation2\n";
  }
};

class ConcreteClass2 : public AbstactClass {
 protected:
  void RequiredOperations1() const override {
    std::cout << "ConcreteClass2 says: Implemented Operation1\n";
  }

  void RequiredOperation2() const override {
    std::cout << "ConcreteClass2 says: Implemented Operation2\n";
  }

  void Hook1() cosnt override {
    std::cout << "ConcreteClass2 says: Overridden Hook1\n";
  }
};

class Person {
 public:
  int age;
};

int main() {
  Person&& p{10};
  auto* concreteClass1 = new ConcreteClass1;
  concreteClass1->TemplateMethod();
  auto* concreteClass2 = new ConcreteClass2;
  concreteClass2->TemplateMethod();
  delete concreteClass1;
  delete concreteClass2;
  return 0;
}
