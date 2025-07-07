#include <iostream>
#include <utility>
#include <vector>

int HPartition(std::vector<int> &nums, int l, int r) {
  // 这种分区方法可以返回一个索引使得左边值都小于等于pivot,右边都大于等于pivot
  //   但是基准值并没有在最终排序位置
  int pivot = nums[l];
  int i = l - 1;
  int j = r + 1;
  while (true) {
    do {
      // 直到找到大于pivot的值
      i++;
    } while (nums[i] < pivot); 
    do {
      j--;
    } while (nums[j] > pivot);
    if (i >= j) {
      return j;
    }
    // 交换
    std::swap(nums[i], nums[j]);
  }
}

int LPartition(std::vector<int> &nums, int l, int r) {
  // 这种分区方法可以返回一个索引使得左边值都小于等于pivot,右边都大于等于pivot
  //   但是基准值并没有在最终排序位置
  int pivot = nums[r];
  int partIndex = l; // 其左边的值小于pivot,右边的值大于等于pivot
  for (int j = l; j < r; ++j) {
    if (nums.at(j) < pivot) {
      // 如果当前值小于基准值,将该值与partIndex交换,使得partIndex左边值小于基准值
      std::swap(nums.at(j), nums.at(partIndex));
      partIndex++;
    }
  }
  //   将基准值排好序
  std::swap(nums.at(r), nums.at(partIndex));
  return partIndex;
}
void quickSort(std::vector<int> &nums, int l, int r) {
  if (l >= r) {
    return;
  }
  int pivot = l;
  int left = l, right = r;
  int pivotValue = nums.at(l);
  while (left < right) {
    while (left < right && nums.at(left) <= pivotValue) {
      left++;
    }

    while (left < right && nums.at(right) >= pivotValue) {
      right--;
    }

    if (left == right) {
      break;
    }
    // 在两侧找到比pivot更小的和比pivot更大的
    std::swap(nums[left], nums[right]);
  }
  // 得到新的pivot,也就是l和r相遇点
  //   如果nums.at(left/right)大于pivotValue,交换后会导致左侧值大于pivotValue,所以将交换的索引-1,使得交换值小于pivotValue
  int newPivot = nums.at(left) <= pivotValue ? left : left - 1;
  std::swap(nums.at(pivot), nums.at(newPivot));
  quickSort(nums, l, newPivot - 1);
  quickSort(nums, newPivot + 1, r);
  return;
}

int main() {
  auto nums = std::vector<int>{3, 1, 5, 4, 6, 7};
  quickSort(nums, 0, nums.size() - 1);
  for (auto num : nums) {
    std::cout << num;
  }
  return 0;
}