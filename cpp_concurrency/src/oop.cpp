#include <iostream>

using namespace std;

class Base {
public:
  Base() { cout << "Base constructor" << endl; }
  Base(int n) : num(n) { cout << "Base constructor" << n << endl; }
  virtual ~Base() { cout << "Base destructor" << endl; }
  int getNum() { return num; }

private:
  int num;
};

class Derived : public Base {
public:
  Derived() { cout << "Derived constructor" << endl; }
  Derived(int n) { cout << "Derived constructor" << n << endl; }
  ~Derived() override { cout << "Derived destructor" << endl; }
};
int f() { return 5; }
int main() {
  const int a = f();
  Base *b = new Derived();
  delete b;
  Derived d{1};
  return 0;
}
