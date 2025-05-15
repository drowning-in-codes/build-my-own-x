#include <iostream>
using namespace std;

int main() {
  int n; // 菜的总数
  cin >> n;
  double total_price{};
  double discount_price{};
  for (int i{}; i < n; i++) {
    double a_i, b_i; // 原价,折扣价
    cin >> a_i >> b_i;
    if (a_i <= 0 || b_i <= 0 ||b_i>a_i) {
      cout << "error\n";
      return 0;
    }
    total_price += a_i;
    discount_price += b_i;
  }
  int x, y; // 满减
  cin >> x >> y;
  if (x < y || x <= 0 || y <= 0) {
    cout << "error\n";
    return 0;
  }
  // 满减
  double final_price = total_price > x ? total_price - y : total_price;
  printf("%.2f\n", min(final_price, discount_price));
  return 0;
}