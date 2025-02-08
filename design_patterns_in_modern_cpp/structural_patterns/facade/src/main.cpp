#include <string>

class Subsystem1 {
 public:
  std::string Operation1() const { return "Subsystem1: Ready!\n"; }

  std::string OperationN() const { return "Subsystem1: Go!\n"; }
};

class Subsystem2 {
 public:
  std::string Operation1() const { return "Subsystem2: Get ready!\n"; }

  std::string OperationN() const { return "Subsystem2: Fire!"; }
};

class Facade {
 protected:
  Subsystem1* subsystem1_;
  Subsystem2* subsystem2_;

 public:
  explicit Facade(Subsystem1* subsystem1 = nullptr,
                  Subsystem2* subsystem2 = nullptr)
      : subsystem1_(subsystem1 ? subsystem1 : new Subsystem1),
        subsystem2_(subsystem2 ? subsystem2 : new Subsystem2) {}

  ~Facade() {
    delete subsystem1_;
    delete subsystem2_;
  }

  std::string Operation() {
    std::string result = "Facade initialized subsystem";
    result += subsystem1_->Operation1();
    result += subsystem2_->Operation1();
    return result;
  }
};

int main() {
  auto* subsystem1 = new Subsystem1;
  auto* subsystem2 = new Subsystem2;
  auto* facade = new Facade(subsystem1, subsystem2);
  delete facade;

  return 0;
}
