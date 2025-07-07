//
// Created by proanimer on 25-6-23.
//

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
/*
 * 水库算法
 */
class Solution {
    ListNode* m_head;
public:
    Solution(ListNode* head):m_head(head) {

    }

    int getRandom() {
        auto cur = m_head;
        int count = 0;
        int result = 0;
        while (cur != nullptr) {
            count++;
            if (rand() % count == 0) {
                result = cur->val;
            }
            cur = cur->next;
        }
        return result;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(head);
 * int param_1 = obj->getRandom();
 */