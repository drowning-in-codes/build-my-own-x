#include <concepts>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>

template <typename T> struct DefaultDeleter {
  void operator()(T *p) const { delete p; }
};
// 偏特化
template <typename T> struct DefaultDeleter<T[]> {
  void operator()(T *p) const { delete[] p; }
};

// 特化
template <> struct DefaultDeleter<FILE> {
  void operator()(FILE *p) const {
    fclose(p);
    if (!p) {
      perror("close error");
    }
  }
};
template <typename T, typename deleter = DefaultDeleter<T>> struct UniquePtr {
private:
  T *m_p;

  template <class U, class UDeleter> friend struct UniquePtr;

public:
  // 通过构造实现
  UniquePtr() { m_p = nullptr; }
  explicit UniquePtr(T *p) { m_p = p; }

  UniquePtr(const UniquePtr &) = delete;
  UniquePtr &operator=(UniquePtr &) = delete;

  UniquePtr(const UniquePtr &&t_p) { m_p = std::exchange(t_p, nullptr); }
  UniquePtr &operator=(UniquePtr &&t_p) {
    // free(m_p);
    deleter{}(m_p);
    m_p = std::exchange(t_p, nullptr);
  }
  ~UniquePtr() {
    // free(m_p);
    deleter{}(m_p);
    m_p = nullptr;
  }
  template <class U, class UDeleter>
    requires(std::convertible_to<U *, T *>)
  UniquePtr(UniquePtr<U, UDeleter> &&that) {
    m_p = std::exchange(that.m_p, nullptr);
  }
  void reset() {
    if (m_p) {
      deleter{}(m_p);
    }
    m_p = nullptr;
  }
  void reset(T *ptr = nullptr) {
    if (m_p) {
      deleter{}(m_p);
    }
    m_p = ptr;
  }

  T &operator*() { return *m_p; }
  T *operator->() const { return m_p; }
  T *release() { return std::exchange(m_p, nullptr); }
};

template <typename T, typename Deleter>
struct UniquePtr<T[], Deleter> : UniquePtr<T, Deleter> {};

class MyClass {
private:
  std::string name;
  int age;

public:
  MyClass() = default;
  MyClass(std::string t_name, int t_age) : name(t_name), age(t_age) {
    puts(__PRETTY_FUNCTION__);
  }
};

template <typename T, typename... Args>
UniquePtr<T> makeUnique(Args &&...args) {
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
UniquePtr<T> makeUniqueForOverwrite(Args... args) {
  return UniquePtr<T>(new T);
}

struct compound {
  int a, b, c;
};

struct Animal {
  virtual void speak() = 0;
  virtual ~Animal() = default;
};

struct Dog : Animal {
  int m_age;
  Dog(int t_age) : m_age(t_age) {}
  virtual void speak() { printf("Dog:I'm %d years old", m_age); }
};

struct Cat : Animal {
  int &m_age;
  Cat(int &t_age) : m_age(t_age) {}
  virtual void speak() { printf("Cat:I'm %d years old", m_age); }
};

void foo(int &&) {}
int main() {
  MyClass mc("", 1);
  // int at = 2;
  // std::cout << &at << "\n";
  // wrapper(std::move(at));
  // wrapper(6);
  // const char(&ra)[6] = "aaaaa"; // 左值 字符串字面量  左值常量引用
  int age = 23;
  std::vector<UniquePtr<Animal>> zoo;
  auto ppc = makeUnique<Dog>(10);
  auto cc = &ppc;
  zoo.push_back(makeUnique<Cat>(age));
  for (auto const &a : zoo) {
    a->speak();
  }
  age++;
  for (auto &a : zoo) {
    a->speak();
  }

  auto pp = makeUnique<MyClass>("s", 12);
  auto p = UniquePtr<MyClass>(new MyClass("aa", 1));
  auto a = UniquePtr<FILE>(fopen("a.txt", "r"));
  return 0;
}
