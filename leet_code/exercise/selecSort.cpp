#include <iostream>
#include <vector>

void selectSort(std::vector<int> &nums) {
  // 选择i+1-n中最小的元素放在i处
  auto s = nums.size();
  for (int i = 0; i < s - 1; ++i) {
    //  3 6 7 9 10    9 
    int minValue = nums.at(i);
    int minIndex = i;
    for (int j = i + 1; j < s; ++j) {
      if (nums.at(j) < minValue) {
        minIndex = j;
        minValue = nums.at(j);
      }
    }
    std::swap(nums.at(i), nums.at(minIndex));
  }
}

int main() {
  std::vector<int> nums{3, 2, 5, 16, 9, 9, 4, 10, 8};
  selectSort(nums);
  for (auto n : nums) {
    std::cout << n << ',';
  }
  return 0;
}