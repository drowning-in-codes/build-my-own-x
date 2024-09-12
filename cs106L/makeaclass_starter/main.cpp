#include "class.h"
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

/*
 * Assigment 3: Make a class!
 * Requirements:
 * Must have a custom constructor ❌
 * Must have a default constructor ❌
    - i.e. constructor overloading
 * Must have private members (functions and/or variables) ❌
 * Must have public members (functions) ❌
 * Must have at least one getter function ❌
 * Must have at least one setter function ❌
 */

int main() {
  // initialize class and run this file
  myConstucor myClass;
  myClass.setX(5);
  std::vector<int> numbers{1, 2, 3};
  std::cout << myClass.getY() << std::endl;
  myClass << std::cout;
  return 0;
}
