#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
class Solution {
public:
  int maxProduct(vector<string> &words) {
    int maxProduct{};
    unordered_map<int, int> nums; // word->length
    for (auto word : words) {
      int r = 0;
      for (auto w : word) {
        int i = w - 'a';
        r |= 1 << i;
      }
      // if (nums.contains(r)) {
      //   if (nums.at(r) < word.size()) {
      //     nums[r] = word.size();
      //   }
      // }
      nums[r] = max(nums[r], static_cast<int>(word.size()));
      for (auto &[w, len] : nums) {
        if ((w & r) == 0) { // 是否有重复字母
          maxProduct = max(maxProduct, len * nums[r]);
        }
      }
    }

    return maxProduct;
  }
};
int main() {}
