#include <vector>

void mergeSort(std::vector<int> &nums, std::vector<int> &cache, int l, int r) {
  if (l >= r) {
    return;
  }
  int mid = (r - l) / 2 + l;
  //   分, 分为两部分
  mergeSort(nums, cache, l, mid);
  mergeSort(nums, cache, mid + 1, r);

  int i = l, j = mid + 1;
  //   解, 将两部分(这两部分分别都是排好序的)排序
  //   递归地对每个子数组进行归并排序
  for (int pos = l; pos <= r; pos++) {
    // i [l,mid]  j [mid+1,r]
    if (j > r /* 右半部分已处理完*/ || (i <= mid && nums.at(i) <= nums.at(j))) {
      cache[pos] = nums.at(i);
      i++;
    } else {
      cache[pos] = nums.at(j);
      j++;
    }
  }

  //   合并两个有序子数组
  // while (i <= mid && j <= right) {
  //     if (arr[i] <= arr[j]) {
  //         temp.push_back(arr[i++]);
  //     } else {
  //         temp.push_back(arr[j++]);
  //     }
  // }

  // // 处理剩余元素
  // while (i <= mid) temp.push_back(arr[i++]);
  // while (j <= right) temp.push_back(arr[j++]);

  //   合并
  for (int i = l; i <= r; i++) {
    nums.at(i) = cache.at(i);
  }
}

int main() {}