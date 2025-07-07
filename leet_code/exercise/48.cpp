#include <vector>
using namespace std;

class Solution {
public:
  void rotate(vector<vector<int>> &matrix) {
    auto s = matrix.size() - 1;
    for (int i = 0; i <= s / 2; ++i) {
      for (int j = i; j < s - i; ++j) {
        int pivot = matrix[i][s - j];
        matrix[i][s - j] = matrix[i][j];
        matrix[i][j] = matrix[s - i][j];
        matrix[s - i][j] = matrix[s - i][s - j];
        matrix[s - i][s - j] = pivot;
      }
    }
  }
};
