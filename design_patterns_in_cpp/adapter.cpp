#include <iostream>
#include <memory>
// Purpose: Allows incompatible interfaces to work together by translating the
// interface of one class into another expected by the client.

// Why It’s Used Today: C++ developers often need to integrate legacy systems or
// third-party libraries that don’t fit into their current architecture. The
// Adapter pattern is essential for bridging these gaps.
class OldInterface {
public:
  void oldMethod() { std::cout << "Old Method" << std::endl; }
};

class NewInterface {
public:
  void virtual newMethod(){};
};

class Adapter : public NewInterface {
private:
  OldInterface *oldInterface;

public:
  Adapter(OldInterface *oldInterface) : oldInterface(oldInterface) {}

  void newMethod() override { oldInterface->oldMethod(); }
};

int main() {
  std::unique_ptr<OldInterface> oldInterface = std::make_unique<OldInterface>();
  auto adapter = Adapter(oldInterface.get());
  adapter.newMethod();
}

// Common Use Cases: Legacy system integration, third-party library
// compatibility.
