#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <vector>
// 模板类定义
template <typename T>
class MyTemplateClass {
private:
    T data;

public:
    MyTemplateClass(const T& val) : data(val) {}

    template <class S>
    // 重载 << 运算符为友元函数
    friend std::ostream& operator<<(std::ostream& os,
                                    const MyTemplateClass<S>& obj);
};

// 模板化的重载 << 运算符定义
template <typename T>
std::ostream& operator<<(std::ostream& os, const MyTemplateClass<T>& obj) {
    os << "MyTemplateClass contains: " << obj.data;
    return os;
}
void printSet(std::set<int> s) {
    for(auto&& i : s) {
        std::cout << i << std::endl;
    }
}
class MyClass {
public:
    int data;

    void changeData(const std::shared_ptr<MyClass>& obj) {
        obj->data = 10;  // 编译通过：data不是const
    }
};

void func(const int& num) {
    std::cout << num << std::endl;
}
void func(int& num) {
    num = 4;
    std::cout << num << std::endl;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // 定义正态分布
    std::normal_distribution<> dist(10.0, 5.0);  // 均值为10.0，标准差为5.0

    // 创建一个vector来存储生成的随机数
    std::vector<double> numbers;

    // 生成100个正态分布的随机数
    const int num_samples = 100;
    for(int i = 0; i < num_samples; ++i) {
        double sample = dist(gen);
        numbers.push_back(sample);
    }

    // 打印生成的随机数
    for(double num : numbers) {
        std::cout << num << std::endl;
    }
    int a = 5;
    std::vector<int> v{ 1, 23 };
    v.push_back(std::move(a));
    a = 7;
    auto i = v;
    for(auto&& num : v) {
        std::cout << num << std::endl;
    }
}
