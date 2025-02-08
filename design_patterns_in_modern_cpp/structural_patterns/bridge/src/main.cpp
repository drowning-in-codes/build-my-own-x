#include <iostream>
#include <string>

// The Pimpl Idiom
struct Person {
  std::string name;
  void greet();
  Person();
  ~Person();

  struct PersonImpl;
  PersonImpl* impl;
};

struct Person::PersonImpl {
  void greet(Person* p) {
    std::cout << "Hello, my name is " << p->name << std::endl;
  }
};

Person::Person() : impl(new PersonImpl) {}

Person::~Person() {
  delete impl;
}

void Person::greet() {
  impl->greet(this);
}

struct Renderer {
  virtual void render_circle(float x, float y, float radius) = 0;
};

struct VectorRenderer : Renderer {
  void render_circle(float x, float y, float radius) override {
    std::cout << "Rasterizing circle of radius " << radius << std::endl;
  }
};

struct RasterRenderer : Renderer {
  void render_circle(float x, float y, float radius) override {
    std::cout << "Drawing a vector circle of radius " << radius << std::endl;
  }
};

struct Shape {
 protected:
  Renderer& renderer;

  explicit Shape(Renderer& renderer) : renderer(renderer) {}

 public:
  virtual void draw() = 0;
  virtual void resize(float factor) = 0;
};

struct Circle : Shape {
  float x, y, radius;
  Circle(Renderer& renderer, float x, float y, float radius)
      : Shape{renderer}, x{x}, y{y}, radius{radius} {};

  void draw() override { renderer.render_circle(x, y, radius); }

  void resize(float factor) override { radius *= factor; }
};

int main() {}