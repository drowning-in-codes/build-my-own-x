module; // 全局模块
#include <string>
#include <vector>
// 声明模块
export module Mod1;
struct Order {
  int count;
  std::string name;
  double price;
  Order(int c, const std::string &n, double p) : count{c}, name{n}, price{p} {}
};

export class Customer {
private:
  std::string name;
  std::vector<Order> orders;

public:
  Customer(const std::string &n) : name(n) {}
  void buy(const std::string &ordername, double price) {
    orders.push_back(Order{1, ordername, price});
  }
  void buy(int num, const std::string &ordername, double price) {
    orders.push_back(Order{num, ordername, price});
  }
  double sumPrice() const;
  double averagePrice() const;
  double price() const;
};
