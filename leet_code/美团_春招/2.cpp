#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  int n, q;
  // 数组大小和询问次数
  cin >> n >> q;
  vector<int> num(n, 0);
  long zero_count{0};
  long sum{};
  for (int i{}; i < n; i++) {
    cin >> num[i];
    if (num[i] == 0) {
      zero_count++;
    }
    sum += num[i];
  }
  for (int i{}; i < q; i++) {
    int  l, r;
    cin >> l >> r;
    long long min = zero_count * l + sum;
    long long max = zero_count * r + sum;
    cout << min << ' ' << max << '\n';
  }
  return 0;
}
