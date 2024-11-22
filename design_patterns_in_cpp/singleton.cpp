
class Singleton {
public:
  static Singleton &getInstance() {
    static Singleton instance;
    return instance;
  }

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

private:
  Singleton() = default;
  // ~Singleton() = default;
};

class PointerSingleton {
public:
  static PointerSingleton *getInstance() { return instance; }

private:
  PointerSingleton() = default;
  ~PointerSingleton() = default;
  static PointerSingleton *instance;
};
PointerSingleton *PointerSingleton::instance{};

int main() {
  auto &singleton = Singleton::getInstance();
  auto *ps = PointerSingleton::getInstance();
}
