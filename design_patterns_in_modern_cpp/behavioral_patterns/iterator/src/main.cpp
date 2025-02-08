#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

template <typename T, typename U>
class Iterator;

// 完整定义 Container 类
template <class T>
class Container {
  friend class Iterator<T, Container>;  // 声明 Iterator 为友元

 public:
  void Add(T a) { m_data_.push_back(a); }

  // 使用智能指针避免内存泄漏
  std::unique_ptr<Iterator<T, Container>> createIterator() {
    return std::make_unique<Iterator<T, Container>>(this);
  }

 private:
  std::vector<T> m_data_;
};

// Iterator 类模板
template <typename T, typename U>
class Iterator {
 public:
  using iter_type = typename std::vector<T>::iterator;

  explicit Iterator(U* p_data, bool /*reverse*/ = false) : m_p_data_(p_data) {
    static_assert(
        std::is_same_v<
            T, typename decltype(std::declval<U>().m_data_)::value_type>,
        "error");
    m_it_ = m_p_data_->m_data_.begin();
  }

  void First() { m_it_ = m_p_data_->m_data_.begin(); }

  void Next() { ++m_it_; }

  bool isDone() const { return (m_it_ == m_p_data_->m_data_.end()); }

  iter_type Current() const { return m_it_; }

 private:
  U* m_p_data_;
  iter_type m_it_;
};

int main() {
  Container<int> cont;
  cont.Add(10);
  auto iterator = cont.createIterator();
  iterator->First();
  while (!iterator->isDone()) {
    std::cout << *iterator->Current() << std::endl;
    iterator->Next();
  }
  return 0;
}
