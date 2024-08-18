/*
Assignment 2: Marriage Pact

This assignment asks you to discover your one true love using containers and
pointers. There are two parts: implement get_applicants and find_matches.

Submit to Paperless by 11:59pm on 2/1/2024.
*/

#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

std::set<std::string> get_applicants(std::string filename) {
  // TODO: Implement this function. Feel free to change std::set to
  // std::unordered_set if you wish!
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << filename << std::endl;
    exit(1);
  }
  std::set<std::string> students;
  std::string name;
  while (std::getline(file, name)) {
    students.insert(name);
  }
  return students;
}

std::unordered_set<std::string> get_unordered_applicants(std::string filename) {
  // TODO: Implement this function. Feel free to change std::set to
  // std::unordered_set if you wish!
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << filename << std::endl;
    exit(1);
  }
  std::unordered_set<std::string> students;
  std::string name;
  while (std::getline(file, name)) {
    students.insert(name);
  }
  return students;
}

std::queue<const std::string *> find_matches(std::set<std::string> &students) {
  // TODO: Implement this function.
  std::queue<const std::string *> matches;
  for (const std::string &student : students) {
    if (student.at(0) == 'S') {
      matches.push(&student);
    }
  }
  return matches;
}

int main() {
  // Your code goes here. Don't forget to print your true love!
  auto students_names = get_applicants("./students.txt");
  // for (auto &s : students_names) {
  //   std::cout << s << std::endl;
  // }
  auto m = find_matches(students_names);
  std::cout << "Match count:" << m.size() << "\n";
  std::random_device rd;
  std::mt19937 gen(rd());
  // 指定随机数的范围
  std::uniform_int_distribution<int> dis(0, m.size() - 1); // 分布范围
  // 生成随机数
  int random_number = dis(gen);
  while (random_number--) {
    m.pop();
  }
  std::cout << "The true one: " << *(m.front());
  return 0;
}
