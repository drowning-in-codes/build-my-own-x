//
// Created by proanimer on 25-6-23.
//

#include <numeric>
#include <vector>

class Solution {
    std::vector<int> m_weights;

public:
    Solution(std::vector<int> &w): m_weights(std::move(w)) {
        /*生成前缀和*/
        std::partial_sum(m_weights.begin(),m_weights.end(),m_weights.begin());
    }

    int pickIndex() {
        /*[1,m_weights[-1]*/
        int randomWeights = (rand() % m_weights.back()) + 1;
        /*
         * 找到>=权重值的索引
         */
        /*
         * lower_bound 用于在有序区间中查找第一个“不小于（≥）给定值”的元素的位置。
         */
        return std::lower_bound(m_weights.begin(),m_weights.end(),randomWeights) - m_weights.begin();
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(w);
 * int param_1 = obj->pickIndex();
 */
