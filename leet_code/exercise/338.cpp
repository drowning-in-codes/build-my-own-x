#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
class Solution {
public:
  vector<int> countBits(int n) {
    vector<int> bits(n + 1, 0);
    for (int i = 1; i <= n; i++) {
      bits[i] = i & 1 ? bits[i - 1] + 1 : bits[i >> 1];
    }
    return bits;
  }
};
