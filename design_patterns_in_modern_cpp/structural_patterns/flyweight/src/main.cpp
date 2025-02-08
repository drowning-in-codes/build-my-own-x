#include <iostream>
#include <ostream>
#include <unordered_map>
#include <utility>

struct SharedState {
  std::string brand_;
  std::string model_;
  std::string color_;

  SharedState(std::string brand, std::string model, std::string color)
      : brand_(std::move(brand)),
        model_(std::move(model)),
        color_(std::move(color)) {}

  friend std::ostream& operator<<(std::ostream& os, const SharedState& ss) {
    return os << "[ " << ss.brand_ << " , " << ss.model_ << " , " << ss.color_
              << " ]";
  }
};

struct UniqueState {
  std::string owner_;
  std::string plates_;

  UniqueState(std::string owner, std::string plates)
      : owner_(std::move(owner)), plates_(std::move(plates)) {}

  friend std::ostream& operator<<(std::ostream& os, const UniqueState& us) {
    return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
  }
};

class Flyweight {
 private:
  SharedState* shared_state_;

 public:
  explicit Flyweight(const SharedState* shared_state)
      : shared_state_(new SharedState(*shared_state)) {}

  Flyweight(const Flyweight& other)
      : shared_state_(new SharedState(*other.shared_state_)) {}

  Flyweight& operator=(const Flyweight& other) {
    if (this != &other) {
      delete shared_state_;
      shared_state_ = new SharedState(*other.shared_state_);
    }
    return *this;
  }

  ~Flyweight() { delete shared_state_; }

  SharedState* shared_state() const { return shared_state_; }

  void Operation(const UniqueState* unique_state) const {
    std::cout << "Flyweight: Display shared(" << *shared_state
              << ") and unique_state:" << *unique_state;
  }
};

class FlyweightFactory {
 private:
  std::unordered_map<std::string, Flyweight> flyweights_;

  std::string GetKey(const SharedState& ss) const {
    return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
  }

 public:
  FlyweightFactory(std::initializer_list<SharedState> shared_states) {
    for (const auto& ss : shared_states) {
      this->flyweights_.emplace(this->GetKey(ss), Flyweight(&ss));
    }
  }

  Flyweight GetFlyweight(const SharedState& shared_state) {
    std::string key = this->GetKey(shared_state);
    if (this->flyweights_.find(key) == this->flyweights_.end()) {
      std::cout
          << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
    } else {
      std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
    }
    return flyweights_.at(key);
  }

  void ListFlyweights() const {
    std::size_t count = flyweights_.size();
    for (const auto& [s, flyweight] : flyweights_) {
      std::cout << count-- << ". " << s << std::endl;
    }
  }
};

void AddCarToPoliceDatabase(FlyweightFactory& ff, const std::string& plates,
                            const std::string& owner, const std::string& brand,
                            const std::string& model,
                            const std::string& color) {
  std::cout << "\nClient: Adding a car to database.\n";
  const Flyweight& flyweight = ff.GetFlyweight({brand, model, color});
  flyweight.Operation(new UniqueState(owner, plates));
}

int main() {
  auto* factory = new FlyweightFactory{{"Che", "fa", "fas"}};
  factory->ListFlyweights();
  AddCarToPoliceDatabase(*factory, "AAA123", "John Doe", "Che", "fa", "fas");
  AddCarToPoliceDatabase(*factory, "BBB123", "John Doe", "Che", "fa", "fas");
  factory->ListFlyweights();
  delete factory;
  return 0;
}
