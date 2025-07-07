#include <vector>
using namespace std;
class Solution {
public:
  vector<int> findDisappearedNumbers(vector<int> &nums) {
    vector<int> res;
    for (int i = 0; i < nums.size(); ++i) {
      // 获得索引
      int index = abs(nums[i]) - 1;
      if (nums.at(index) > 0) {
        // 该值取负, 方便后续判断存在该索引对应的值
        nums.at(index) = -nums.at(index);
      }
    }
    for (int i = 0; i < nums.size(); ++i) {
      if (nums.at(i) > 0) {
        res.push_back(i + 1);
      }
    }

    return res;
  }
};
