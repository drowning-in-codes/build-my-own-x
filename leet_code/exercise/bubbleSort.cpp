#include <iostream>
#include <vector>

void bubbleSort(std::vector<int> &nums) {
  auto s = nums.size();
  for (int i = 0; i < s - 1; i++) {
    // 从序列第一个元素开始遍历,如果大于后一个元素则交换位置
    // 每次遍历将最大元素放在末尾,末尾序列元素已被排序完毕
    bool swapped{};
    for (int j = 0; j < s - 1 - i; j++) {
      // 遍历未被排序的序列
      if (nums.at(j) > nums.at(j + 1)) {
        std::swap(nums.at(j), nums.at(j + 1));
        // 优化,如果在未被排序的序列中被swap,表明已经排好顺序
        swapped = true;
      }
    }
    if (!swapped) {
      break;
    }
  }
}
int main() {
  std::vector<int> nums{3, 2, 5, 16, 7, 9, 4, 10, 8};
  bubbleSort(nums);
  for (auto n : nums) {
    std::cout << n;
  }
  return 0;
}