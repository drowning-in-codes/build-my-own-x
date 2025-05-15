#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> cal_prefix(const vector<vector<int>> &matrix) {
  int n = matrix.size();
  vector<vector<int>> prefix_sum(n + 1, vector<int>(n + 1, 0));
  for (size_t i = 1; i <= n; i++) {
    for (size_t j = 1; j <= n; j++) {
      prefix_sum[i][j] = prefix_sum[i - 1][j] + prefix_sum[i][j - 1] -
                         prefix_sum[i - 1][j - 1] + matrix[i - 1][j - 1];
    }
  }
  return prefix_sum;
}

int main() {
  int n;
  cin >> n;
  vector<vector<int>> matrix(n, vector<int>(n));
  for (size_t i = 0; i < n; i++) {
    std::string line_value;
    cin >> line_value;
    for (size_t j = 0; j < n; j++) {
      int bit = line_value.at(j) - '0';
      matrix[i][j] = bit;
    }
  }

  vector<vector<int>> prefix_sum = cal_prefix(matrix);
  int perfect_area = 0;
  // vector<int> perfect_area_num;
  // for special index = 0
  for (size_t index = 1; index <= n; index++) {
    perfect_area = 0;
    for (size_t i = 0; i <= n - index; i++) {
      for (size_t j = 0; j <= n - index; j++) {
        int ones = prefix_sum[i + index][j + index] - prefix_sum[i + index][j] -
                   prefix_sum[i][j + index] + prefix_sum[i][j];
        int zeros = index * index - ones;
        if (ones == zeros) {
          perfect_area++;
        }
      }
    }
    cout << perfect_area << '\n';
  }

  return 0;
}
