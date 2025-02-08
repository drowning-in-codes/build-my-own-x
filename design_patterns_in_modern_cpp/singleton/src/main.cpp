#include <atomic>
#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

struct Database {
 private:
  static std::atomic<Database*> instance;
  static std::mutex mutex;

 protected:
  Database() = default;

 public:
  static Database& get_instance() {
    // static Database instance;
    // return instance;
    Database* db = instance.load(std::memory_order_consume);
    if (!db) {
      std::lock_guard<std::mutex> lock(mutex);
      db = instance.load(std::memory_order_consume);
      if (!db) {
        db = new Database();
        instance.store(db, std::memory_order_release);
      }
    }
    return *db;
  }

  Database(const Database&) = delete;
  Database& operator=(const Database&) = delete;
  Database(Database&&) = delete;
  Database& operator=(Database&&) = delete;
};

class DB {
 public:
  virtual int get_population(const std::string& name) = 0;
  virtual ~DB() = default;
};

class SingletonDatabase : public DB {
  SingletonDatabase() = default;

  std::map<std::string, int> capitals;

 public:
  SingletonDatabase(SingletonDatabase const&) = delete;
  void operator=(SingletonDatabase const&) = delete;

  static SingletonDatabase& get() {
    static SingletonDatabase db;
    return db;
  }

  int get_population(const std::string& name) override {
    return capitals[name];
  }
};

struct SingletonRecordFinder {
  DB& db;

  explicit SingletonRecordFinder(DB& db) : db{db} {}

  int total_population(std::vector<std::string> names) {
    int result{};
    for (auto& name : names) {
      result += db.get_population(name);
    }
    return result;
  }
};

class dummyDatabase : public DB {
  std::map<std::string, int> capitals;

 public:
  dummyDatabase() {
    capitals["alpha"] = 1;
    capitals["beta"] = 2;
    capitals["gamma"] = 3;
  }

  int get_population(const std::string& name) override {
    return capitals[name];
  }
};

class PerThreadSingleton {
  PerThreadSingleton() { id = std::this_thread::get_id(); }

 public:
  std::thread::id id;

  static PerThreadSingleton& get() {
    thread_local PerThreadSingleton instance;
    return instance;
  }
};

int main() {
  auto& db = SingletonDatabase::get();
  return 0;
}