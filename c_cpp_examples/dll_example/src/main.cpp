#include "mathcal.hpp"
#include "sysinfoapi.h"
#include <cstdlib>
#include <iostream>
#include <windows.h>

int main() {
  std::cout << file_name;
  say_hello();
  change_file_name();
  std::cout << file_name;
  constexpr std::size_t N = 50;
  char dir[N] = {};
  UINT nLne = GetSystemDirectoryA(dir, N);
  if (nLne == 0) {
    std::cerr << "Error: " << GetLastError() << std::endl;
    return 1;
  }
  std::cout << dir << '\n';
  nLne = GetWindowsDirectory(dir, N);
  if (nLne == 0) {
    std::cerr << "Error: " << GetLastError() << std::endl;
    return 1;
  }

  std::cout << dir;
}
