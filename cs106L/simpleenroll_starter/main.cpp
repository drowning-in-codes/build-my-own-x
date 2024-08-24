/*
 * CS106L SimpleEnroll Assignment Part 1
 * Created by Fabio Ibanez
 */

#include "utils.cpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <vector>

std::string COURSES_PATH =
    "E:/build-my-own-x/cs106L/simpleenroll_starter/courses.csv";

bool structTest() {
  Course c{"Standard C++ Programming", "1", "2023-2024 Winter"};
  return true;
}

int main() {
  std::vector<Course> vector_of_courses;

  if (!structTest()) {
    std::cerr << "structTest Failed 😞";
  } else {
    std::cout << "structTest Passed! ✅" << '\n';
  }

  parse_csv(COURSES_PATH, vector_of_courses);

  write_courses_offered(vector_of_courses);

  write_courses_not_offered(vector_of_courses);

  FILE *pipe = popen("python3 utils/autograder.py", "r");
  if (!pipe) {
    std::clog << "Error";
    throw std::runtime_error("popen failed");
    return -1;
  }

  char buffer[256];
  while (!feof(pipe)) {
    if (fgets(buffer, 256, pipe) != NULL)
      std::cout << buffer;
  }
  pclose(pipe);

  // uncomment this line to debug
  // print_vector(vector_of_courses);
  return 0;
}
