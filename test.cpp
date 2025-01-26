
// void foo(int &a) {}
// void foo(int & a){}
#include <cstdio>
#include <iostream>

#define concat(x, y) x##y
#define makestr(x) #x

class Person {
public:
  Person() { std::cout << "Person()" << std::endl; }
  Person(const Person &p){

  };

  ~Person() { std::cout << "~Person()" << std::endl; }
};
// Person GetPerson() {
//   Person p; // NRVO and copy elision
//   return p;
// }
//
int main() {
  Person p{};
  Person p1 = std::move(p);
  return 0;
}
