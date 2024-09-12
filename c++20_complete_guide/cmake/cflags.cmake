set(CMAKE_C_COMPILER "C://Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C://Program Files/LLVM/bin/clang++.exe")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SANITIZING_FLAGS -fno-sanitize-recover=all -fsanitize=undefined -fsanitize=address)
# ask for more warnings from the compiler
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Weffc++ -Werror -Wshadow -Wpointer-arith -Wcast-qual -Wformat=2 -Wno-unqualified-std-cast-call -Wno-non-virtual-dtor")
endif()
set(CMAKE_BASE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
