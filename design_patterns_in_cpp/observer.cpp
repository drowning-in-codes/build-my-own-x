#include <vector>
// Purpose: Establishes a one-to-many relationship between objects so that when
// one object changes state, its dependents are notified automatically.

// Why It’s Used Today: This pattern is indispensable in event-driven systems,
// especially in GUIs, real-time data processing, and asynchronous applications.
class Observer {
public:
  virtual void upadate() = 0;
};

class Subject {
private:
  std::vector<Observer *> observers;

public:
  void addObserver(Observer *observer) { observers.push_back(observer); }
  void notifyObservers() {
    for (auto observer : observers) {
      observer->upadate();
    }
  }
};

int main() {}
// Common Use Cases: Event-driven systems, real-time applications, user
// interfaces (UIs).
