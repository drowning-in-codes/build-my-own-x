#include <filesystem>
#include <iostream>

class Subject {
 public:
  virtual void Request() const = 0;
  virtual ~Subject() = default;
};

class RealSubject : public Subject {
 public:
  void Request() const override {
    std::cout << "RealSubject: Handling request.\n";
  }
};

class Proxy : public Subject {
 private:
  RealSubject* real_subject_;

  bool CheckAccess() const {
    std::cout << "Proxy: Checking access prior to firing a real request.\n";
    return true;
  }

  void LogAccess() const {
    std::cout << "Proxy: Logging the time of request.\n";
  }

 public:
  explicit Proxy(RealSubject* real_subject)
      : real_subject_(new RealSubject(*real_subject)) {}

  ~Proxy() override { delete real_subject_; }

  void Request() const override {
    if (this->CheckAccess()) {
      real_subject_->Request();
      LogAccess();
    }
  }
};

int main() {
  auto* real_subject = new RealSubject;
  real_subject->Request();

  auto* proxy = new Proxy(real_subject);
  proxy->Request();
  delete real_subject;
  delete proxy;
  return 0;
}
