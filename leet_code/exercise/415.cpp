#include <functional>
#include <string>
#include <strstream>
//
// Created by proanimer on 25-6-23.
//
class Solution {
public:
    std::string addStrings(string num1, string num2) {
        auto len1 = num1.length();
        auto len2 = num2.length();
        std::reverse(num1.begin(), num1.end());
        std::reverse(num2.begin(), num2.end());
        if (len1 < len2) {
            std::swap(len1, len2);
            std::swap(num1, num2);

        }
        std::string result;
     // num1 "123" num2 "11"
        // 321 11
        bool increasing{false};
        for (int i = 0;i<len1;i++) {
            int num = num1.at(i) - '0' + (i<len2 ? num2.at(i) - '0' : 0);
            if (increasing) {
                num +=1;
            }
            int curBit = num;
            if (num >=10) {
                increasing = true;
                curBit = num%10;
            }else {
                increasing = false;
            }
            result = std::to_string(curBit) + result;
        }
        if (increasing) {
            result = "1"+result;
        }
        return result;
    }
};