
// Common Use Cases: GUI systems, object-oriented frameworks, dynamic object
// creation.

class Product {
public:
  virtual void use() = 0;
};

class ConcreteProduct : public Product {
  void use() override { /* Implementation */ }
};

// Purpose: Provides an interface for creating objects, allowing subclasses to
// alter the type of objects that will be created.

// Why It’s Used Today: It’s widely applied in scenarios where different types
// of objects need to be created dynamically based on runtime conditions. This
// is critical in frameworks and libraries for managing dynamic object creation.
class Factory {
public:
  static Product *createProduct(int type) {
    if (type == 1) {
      return new ConcreteProduct();
    }
    return nullptr;
  }
};

int main() { auto *product = Factory::createProduct(1); }
