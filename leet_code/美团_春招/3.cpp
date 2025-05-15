#include <iostream>
using namespace std;

int main() {
  int n, k; // 字符串长度和操作次数
  cin>>n>>k;
  string input_str;
  cin >> input_str;
  int count = 0;

  for (char c : input_str) {
    if (c == 'M' || c == 'T') {
      count++;
    }
  }
  cout << min(count + k, n);

  return 0;
}