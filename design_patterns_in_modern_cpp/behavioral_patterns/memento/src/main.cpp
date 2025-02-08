#include <cstddef>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Memento {
 public:
  virtual std::string GetName() const = 0;
  virtual std::string date() const = 0;
  virtual std::string state() const = 0;
  virtual ~Memento() = default;
};

class ConcreteMemento : public Memento {

  std::string state_;
  std::string data_;

 public:
  explicit ConcreteMemento(std::string state) : state_{std::move(state)} {
    std::time_t now = std::time(nullptr);
    data_ = std::ctime(&now);
  }

  /*
   * @brief [TODO:description]
   *
   * @return [TODO:return]
   */
  std::string state() const override { return state_; }

  std::string GetName() const override {
    return this->data_ + " / (" + state_.substr(0, 9) + "...)";
  }

  std::string date() const override { return this->data_; }
};

class Originator {
 private:
  std::string state_;

  static std::string GeneraRandomString(int length = 0) {
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::size_t stringLength = sizeof(alphanum) - 1;
    std::string random_string;
    for (std::size_t i{}; i < length; i++) {
      random_string += alphanum[std::rand() % stringLength];
    }
    return random_string;
  }

 public:
  explicit Originator(std::string state) : state_{std::move(state)} {
    std::cout << "Originator: My initial state is: " << state_ << '\n';
  }

  /**
   * The Originator's business logic may affect its internal state. Therefore,
   * the client should backup the state before launching methods of the business
   * logic via the save() method.
   */
  void DoSomething() {
    std::cout << "Originator: I'm doing something important.\n";
    state_ = GeneraRandomString(30);
    std::cout << "Originator: and my state has changed to: " << state_ << '\n';
  }

  Memento* Save() const { return new ConcreteMemento(state_); }

  void Restore(Memento* memento) {
    state_ = memento->state();
    std::cout << "Originator: My state has changed to: " << state_ << '\n';
  }
};

class Caretaker {
 private:
  std::vector<Memento*> mementos_;
  Originator* originator_;

 public:
  explicit Caretaker(Originator* originator) : originator_{originator} {}

  ~Caretaker() {
    for (auto* m : mementos_)
      delete m;
  }

  void Backup() {
    std::cout << "\nCaretaker: Saving Originator's state...\n";
    this->mementos_.push_back(this->originator_->Save());
  }

  void Undo() {
    if (!mementos_.size()) {
      return;
    }
    Memento* memento = mementos_.back();
    this->mementos_.pop_back();
    try {
      originator_->Restore(memento);
    } catch (...) {
      Undo();
    }
  }

  void ShowHistory() const {
    std::cout << "Caretaker: Here's the list of mementos:\n";
    for (auto* memento : mementos_) {
      std::cout << memento->GetName() << '\n';
    }
  }
};

int main() {
  auto* originator = new Originator("Super-duper-super-puper-super.");
  auto* caretaker = new Caretaker(originator);
  caretaker->Backup();
  originator->DoSomething();
  caretaker->Backup();
  originator->DoSomething();
  delete originator;
  delete caretaker;
}
