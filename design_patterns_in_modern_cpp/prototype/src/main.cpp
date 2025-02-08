#include <iostream>
#include <utility>

class Address {
 public:
  std::string street, city;
  int suite;

  Address(std::string street, std::string city, int suite)
      : street{std::move(street)}, city{std::move(city)}, suite{suite} {}

  virtual Address* clone() { return new Address{street, city, suite}; }

  virtual ~Address() = default;
};

class Contact {
 public:
  std::string name;
  Address* address;

  Contact(std::string name, Address* address)
      : name{std::move(name)}, address{address} {}

  Contact(const Contact& other)
      : name{other.name}, address{new Address{*other.address}} {}

  Contact& operator=(const Contact& other) {
    if (&other == this) {
      return *this;
    }
    name = other.name;
    address = new Address{*other.address};
    return *this;
  }

  ~Contact() { delete address; }
};

class ExtendedAddress : public Address {
 public:
  std::string country, postcode;

  ExtendedAddress(const std::string street, const std::string city, int suite,
                  std::string country, std::string postcode)
      : Address{street, city, suite},
        country{std::move(country)},
        postcode{std::move(postcode)} {}

  ExtendedAddress* clone() override {
    return new ExtendedAddress{street, city, suite, country, postcode};
  }
};

class EmployeeFactory {
  static Contact main_;
  static Contact aux_;

  static std::unique_ptr<Contact> NewEmployee(std::string name, int suite,
                                              Contact& proto) {
    auto result = std::make_unique<Contact>(proto);
    result->name = std::move(name);
    result->address->suite = suite;
    return result;
  }

 public:
  static std::unique_ptr<Contact> NewMainOfficeEmployee(std::string name,
                                                        int suite) {
    return NewEmployee(std::move(name), suite, main_);
  }

  static std::unique_ptr<Contact> NewAuxOfficeEmployee(std::string name,
                                                       int suite) {
    return NewEmployee(std::move(name), suite, aux_);
  }
};

Contact EmployeeFactory::main_{"", new Address{"123 East Dr", "London", 0}};
Contact EmployeeFactory::aux_{"", new Address{"123B East Dr", "London", 0}};

int main() {
  Contact worker{"", new Address{"123 East Dr", "London", 0}};
  Contact john{worker};
  john.name = "John Doe";
  ExtendedAddress ea = {"123 East Dr", "London", 0, "UK", "SW1A 1AA"};
  Address& a = ea;

  [[maybe_unused]]
  auto* cloned = a.clone();
  auto jane = EmployeeFactory::NewMainOfficeEmployee("Jane Smith", 100);
  return 0;
}