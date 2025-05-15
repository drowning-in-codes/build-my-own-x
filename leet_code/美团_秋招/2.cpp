#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
  int n; // 字符串长度
  cin >> n;
  string s, t;
  cin >> s;
  cin >> t;
  int sim{};
  vector<int> unmatched_s(26, 0), unmatched_t(26, 0);
  vector<pair<int, int>> to_swap;
  for (int i{}; i < n; i++) {
    if (s[i] == t[i]) {
      sim++;
    } else {
      unmatched_s[s[i] - 'a']++;
      unmatched_t[t[i] - 'a']++;
      to_swap.push_back({s[i] - 'a', t[i] - 'a'});
    }
  }
  int max_increase = 0;
  for (auto &p : to_swap) {
    const auto &[s, t] = p;
    if (unmatched_s[t] > 0 && unmatched_t[s] > 0) {
      max_increase = max(max_increase, 2);
    } else if (unmatched_s[t] > 0 || unmatched_t[s] > 0) {
      max_increase = max(max_increase, 1);
    }
  }
  cout << (sim + max_increase);
  return 0;
}
// 64 位输出请用 printf("%lld")