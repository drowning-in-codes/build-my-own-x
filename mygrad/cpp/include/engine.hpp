#pragma once
#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

using Backward = std::function<void(void)>;

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class Value {
public:
    T data;
    float grad;
    Backward backward_fn;
    std::unordered_set<Value<T>*> prev;
    std::string op;

public:
    Value() = default;
    // 构造函数
    Value(T data, float grad, std::unordered_set<Value<T>*> prev = {},
          std::string op = "", Backward backward_fn = nullptr)
        : data(data), grad(grad), backward_fn(backward_fn), prev(prev), op(op) {
    }
    // 构造函数
    Value(T data, std::unordered_set<Value<T>*> prev = {}, std::string op = "",
          Backward backward_fn = nullptr)
        : data(data), grad(0.0f), backward_fn(backward_fn), prev(prev), op(op) {
    }
    // 拷贝构造函数
    Value(const Value<T>& other);
    // 拷贝赋值
    Value& operator=(const Value<T>&);
    // 运算符重载
    Value operator-();  // 负数重载

    Value operator+(const T&);
    Value operator-(const T&);
    Value operator*(const T&);
    Value operator/(const T&);

    Value operator+(Value<T>&);
    Value& operator+=(Value<T>&);
    Value operator-(Value<T>&);
    Value& operator-=(Value<T>&);
    Value operator*(Value<T>&);
    Value& operator*=(Value<T>&);
    Value operator/(Value<T>&);
    Value& operator/=(Value<T>&);
    // 成员函数
    Value pow(int n);
    Value ReLU();
    Value exp();
    Value tanh();
    Value sigmoid();

    std::vector<Value<T>> toList();
    // 计算梯度
    void backward();
    // 析构方法
    ~Value() = default;
    // 友元方法
    template <typename S>
    friend std::ostream& operator<<(std::ostream&, const Value<S>&);
    friend Value<T> operator+(T data, const Value<T>&);
    friend Value<T> operator-(T data, const Value<T>&);
    friend Value<T> operator*(T data, const Value<T>&);
    friend Value<T> operator/(T data, const Value<T>&);
};

template <typename T, typename U>
Value<T> operator+(T data, const Value<T>& other) {
    Value<T> out = other + data;
    return out;
}

template <typename T, typename U>
Value<T> operator-(T data, const Value<T>& other) {
    Value<T> out = data + (-other);
    return out;
}
template <typename T, typename U>
Value<T> operator*(T data, const Value<T>& other) {
    Value<T> out = other * data;
    return out;
}
template <typename T, typename U>
Value<T> operator/(T data, const Value<T>& other) {
    Value<T> out = data * other.pow(-1);
    return out;
}

template <typename S>
std::ostream& operator<<(std::ostream& os, const Value<S>& value) {
    os << std::format("Value(data={},value={})", value.data, value.grad);
    return os;
}
// 拷贝构造方法
template <typename T, typename U>
Value<T, U>::Value(const Value<T>& other)
    : data(other.data), grad(other.grad), backward_fn(other.backward_fn),
      prev(other.prev), op(other.op) {}

// 拷贝赋值方法
template <typename T, typename U>
Value<T, U>& Value<T, U>::operator=(const Value<T>& other) {
    if(&other == this) return *this;
    this->data = other.data;
    this->grad = other.grad;
    this->backward_fn = other.backward_fn;
    this->prev = other.prev;
    this->op = other.op;
    return *this;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator+(const T& data) {
    Value<T> out(data);
    return *this + out;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator-(const T& data) {
    Value<T> out(data);
    return *this - out;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator*(const T& data) {
    Value<T> out(data);
    return *this * out;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator/(const T& data) {
    Value<T> out(data);
    return *this / out;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator+(Value<T>& other) {
    T value = this->data + other.data;
    Value<T> out(value, std::unordered_set<Value<T>*>{ this }, "+");
    auto backward_fn = [ this, other, &out ]() mutable {
        this->grad += out.grad;
        other.grad += out.grad;
    };
    out.backward_fn = backward_fn;
    return out;
}
template <typename T, typename U>
Value<T, U>& Value<T, U>::operator+=(Value<T>& other) {
    *this = *this + other;
    return *this;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator-(Value<T>& other) {
    Value<T> out = other + (-*this);
    return out;
}
template <typename T, typename U>
Value<T, U>& Value<T, U>::operator-=(Value<T>& other) {
    *this = *this - other;
    return *this;
}
// 负数重载
template <typename T, typename U>
Value<T, U> Value<T, U>::operator-() {
    auto out = *this * -1;
    return out;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::operator*(Value<T>& other) {
    T value = this->data * other.data;
    Value<T> out(value, { this, &other }, "*");
    auto backward_fn = [ this, other, &out ]() mutable {
        this->grad += other.data * out.grad;
        other.grad += this->data * out.grad;
    };
    out.backward_fn = backward_fn;
    return out;
}
template <typename T, typename U>
Value<T, U>& Value<T, U>::operator*=(Value<T>& other) {
    *this = *this * other;
    return *this;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::pow(int n) {
    T value = std::pow(this->data, n);
    Value<T> out(value, { this }, std::format("**{}", n));
    auto backward_fn = [ this, n, &out ]() mutable {
        this->grad += n * std::pow(this->data, n - 1) * out.grad;
    };
    out.backward_fn = backward_fn;
    return out;
}

template <typename T, typename U>
Value<T, U> Value<T, U>::operator/(Value<T>& other) {
    Value<T> out = other.pow(-1);
    return *this * (out);
}
template <typename T, typename U>
Value<T, U>& Value<T, U>::operator/=(Value<T>& other) {
    *this = *this / other;
    return *this;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::ReLU() {
    T value = this->data > T(0) ? this->data : T(0);
    Value<T> out(value, { this }, "ReLU");
    auto backward_fn = [ this, &out ]() mutable {
        this->grad += this->data > T(0) ? out.grad : 0;
    };
    out.backward_fn = backward_fn;
    return out;
}
template <typename T, typename U>
Value<T, U> Value<T, U>::exp() {
    T value = std::exp(static_cast<double>(this->data));
    Value<T> out(value, { this }, "exp");
    auto backward_fn = [ this, &out ]() mutable {
        this->grad += out.grad * static_cast<float>(value);
    };
    out.backward_fn = backward_fn;
    return out;
}

template <typename T, typename U>
Value<T, U> Value<T, U>::tanh() {
    T value = (std::exp(this->data) - std::exp(-this->data))
        / (std::exp(this->data) + std::exp(-this->data));
    Value<T> out(value, { this }, "tanh");
    auto backward_fn = [ this, &out ]() mutable {
        this->grad += out.grad * (1 - out.data * out.data);
    };
    out.backward_fn = backward_fn;
    return out;
}

template <typename T, typename U>
Value<T, U> Value<T, U>::sigmoid() {
    T value = 1 / (1 + std::exp(this->data));
    Value<T> out(value, { this }, "sigmoid");
    auto backward_fn = [ this, &out ]() mutable {
        this->grad += out.grad * out.data * (1 - out.data);
    };
    out.backward_fn = backward_fn;
    return out;
}

template <typename T, typename U>
std::vector<Value<T>> Value<T, U>::toList() {
    return std::vector<Value<T>>{ *this };
}
template <typename T, typename U>
void Value<T, U>::backward() {
    std::vector<Value<T>*> topo;
    std::unordered_set<Value<T>*> visited;
    std::function<void(Value<T>*)> build_topo = [ & ](Value<T>* v) {
        if(not visited.contains(v)) {
            visited.insert(v);
            for(auto&& p : v->prev) {
                build_topo(p);
                topo.push_back(p);
            }
        }
    };
    build_topo(this);
    this->grad = 1;
    for(auto it = topo.rbegin(); it != topo.rend(); ++it) {
        Value<T>* v = *it;
        v->backward_fn();
    }
}
