module Mod1;
double Customer::sumPrice() const {
  double sum = .0;
  for (const Order &od : orders) {
    sum += od.count * od.price;
  }
  return sum;
}

double Customer::averagePrice() const {
  if (orders.empty()) {
    return .0;
  }
}
