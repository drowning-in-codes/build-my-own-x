#pragma once
#include <string>
template <typename T1, typename T2> auto sum(T1 a, T2 b) -> decltype(a + b) {
  return a + b;
}

void say_hello();
extern std::string
    file_name;       // external linkage, can share across multiple files
static int num = 12; // internal linkage, can't share across multiple files.
                     // each file has its own copy of num
inline float weight =
    12.F; // inline variable doesn't affect linkage, but can have
          // multiple but same definitions in header files

inline static int val = 13;

void change_file_name();
