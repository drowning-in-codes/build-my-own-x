// Common Use Cases: Logging systems, database connections, configuration
// management.
class Singleton {
private:
  static Singleton instance;
  Singleton() = default;
  ~Singleton() = default;

public:
  static Singleton &getInstance() { return instance; };
  // 禁止拷贝构造和赋值
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
};

Singleton Singleton::instance;

class PointerSingleton {
private:
  PointerSingleton() = default;
  static PointerSingleton *instance;
};
// Purpose: Ensures that a class has only one instance while providing a global
// point of access to it.

// Why It’s Used Today: The Singleton pattern remains prevalent because many
// systems still need a single, global instance for resource management,
// configuration settings, or logging systems.
class BetterSingleton {
private:
  BetterSingleton() {}
  ~BetterSingleton() {}

public:
  static BetterSingleton &getInstance() {
    static BetterSingleton bs;
    return bs;
  }
  // 禁止拷贝构造和赋值
  BetterSingleton(const BetterSingleton &) = delete;
  BetterSingleton &operator=(const BetterSingleton &) = delete;
};

int main() { auto &singleton = Singleton::; }
