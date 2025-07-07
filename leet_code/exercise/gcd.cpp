#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

int gcd(int num1, int num2) {
    // while (num2 != 0) {
    //   int temp = num2;
    //   num2 = num1 % num2;
    //   num1 = temp;
    // }
    // return num1;
    return num2 == 0 ? num1 : gcd(num2, num1 % num2);
}

// 10进制转7进制
std::string base_7(int num) {
    if (num == 0) {
        return "0";
    }
    int abs_num = std::abs(num);

    int p = num;
    std::string result;
    while (p != 0) {
        int remainder = p % 7;
        int p = p / 7;
        result = std::to_string(remainder) + result;
    }
    if (num < 0) {
        result = "-" + result;
    }
    return result;
}

int lcm(int num1, int num2) { return num1 * num2 / gcd(num1, num2); }

// class Solution {
//     std::vector<int> nums;
//
// public:
//     explicit Solution(std::vector<int> weights): nums(std::move(weights)) {
//         std::partial_sum(nums.begin(), nums.end(), nums.begin());
//     }
//
//     int pickIndex() {
//         std::random_device random_device;
//         std::mt19937 generator(random_device()());
//         std::uniform_int_distribution<> distribution(1, nums.back());
//         int val = distribution(generator);
//         return static_cast<int>(std::lower_bound(nums.begin(), nums.end(), val) - nums.begin());
//     }
// };

class Solution {
    std::vector<int> m_nums;
public:
    Solution(std::vector<int> &nums): m_nums(nums) {

    }
    std::vector<int> reset() {
        return m_nums;
    }
    std::vector<int> shuffle() {
        std::vector<int> result = m_nums;
        for (int i = static_cast<int>(m_nums.size()) - 1; i >= 0; i--) {
            auto index = rand()%(i+1);
            std::swap(result.at(i), result.at(index));
        }
        // for (int i =0; i < m_nums.size(); i++) {
        //     int pos = rand() % (m_nums.size() - i);
        //     std::swap(result.at(i), result.at(i+pos));
        // }
        return result;
    }
};



int main() {
    std::cout << gcd(24, 72);
}
