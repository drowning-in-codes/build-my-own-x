#include <iostream>
#include <list>
#include <string>

class IObserver {
 public:
  virtual void Update(std::string message_from_subject) = 0;
  virtual ~IObserver() = default;
};

class ISubject {
 public:
  virtual ~ISubject() = default;
  virtual void Attach(IObserver*) = 0;
  virtual void Detach(IObserver*) = 0;
  virtual void Notify() = 0;
};

class Subject : public ISubject {
 private:
  std::list<IObserver*> list_observer_;
  std::string message_;

 public:
  void Attach(IObserver* observer) override {
    list_observer_.push_back(observer);
  }

  void Detach(IObserver* observer) override { list_observer_.remove(observer); }

  void Notify() override {
    HowManyObservers();
    for (auto* observer : list_observer_) {
      observer->Update(message_);
    }
  }

  void HowManyObservers() const {
    std::cout << "There are " << list_observer_.size()
              << " observers in the list\n";
  }

  void SomeBussinessLogic() {
    message_ = "change message message";
    Notify();
    std::cout << "I'm about to do some thing important\n";
  }
};

class Observer : public IObserver {
 public:
  explicit Observer(Subject& subject) : subject_(subject) {
    this->subject_.Attach(this);
    std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_
              << "\".\n";
    this->number_ = Observer::static_number_;
  }

  void Update(std::string message_from_subject) override {
    message_from_subject_ = message_from_subject;
    PrintInfo();
  }

  void RemoveMeFromTheList() { this->subject_.Detach(this); }

  void PrintInfo() const {
    std::cout << "Observer \"" << this->number_
              << "\": a new message is available --> "
              << this->message_from_subject_ << "\n";
  }

 private:
  Subject& subject_;
  std::string message_from_subject_;
  static int static_number_;
  int number_;
};

int Observer::static_number_ = 0;

int main() {

  return 0;
}
