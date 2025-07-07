#include <iostream>
#include <vector>

void insertSort(std::vector<int> &nums) {
  auto s = nums.size();
  for (int i = 1; i < s; i++) {
    // 从i之前的元素到0,判断如果小于,则插入到之前
    // 序列前面的元素是已经排好序的
    int temp = i - 1;
    int sortedValue = nums.at(i);
    while (temp >= 0) {
      if (nums.at(temp) > sortedValue) {
        // 6 3
        //  6  6 7
        // std::swap(nums.at(temp), nums.at(i));
        nums.at(temp + 1) = nums.at(temp);
        temp--;
      } else {
        break;
      }
    }
    nums.at(temp + 1) = sortedValue;
  }
}

int main() {
  std::vector<int> nums{3, 2, 5, 16, 7, 9, 4, 10, 8};
  insertSort(nums);
  for (auto n : nums) {
    std::cout << n;
  }
  return 0;
}