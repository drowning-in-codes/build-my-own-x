class Strategy {
public:
  virtual void execute() = 0;
};

// Purpose: Allows for the definition of a family of algorithms, encapsulating
// each one, and making them interchangeable at runtime.

// Why It’s Used Today: The Strategy pattern is often employed in
// performance-critical systems where different algorithms need to be selected
// dynamically at runtime, such as sorting or data processing.
class ConcreteStrategyA : public Strategy {
public:
  void execute() override { /* Implementation A */ }
};
class Context {
private:
  Strategy *strategy;

public:
  void setStrategy(Strategy *strategy) { this->strategy = strategy; }
  void executeStrategy() {
    if (strategy) {
      strategy->execute();
    }
  }
};
// Common Use Cases: Data processing, sorting algorithms, encryption techniques.
int main() {}
