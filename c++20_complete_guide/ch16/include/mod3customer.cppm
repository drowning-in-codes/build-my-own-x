module;
#include <string>
#include <vector>

export module Mod3:Customer;

import :Order;
export class Customer {
private:
  std::string name;
  std::vector<Order> orders;

public:
  Customer(const std::string &n) : name(n);
}
