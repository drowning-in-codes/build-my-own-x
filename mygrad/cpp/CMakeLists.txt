cmake_minimum_required(VERSION 3.28)

project(
cppgrad
VERSION 0.1
DESCRIPTION "A simple C++ library for automatic differentiation"
LANGUAGES CXX)

SET (CMAKE_C_COMPILER  "C:/Program Files/LLVM/bin/clang.exe")
SET (CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

if (${ONLYTEST})
  add_executable(test test.cpp)
  target_compile_features(test PRIVATE cxx_std_20)
else()
  add_library(mygrad INTERFACE)
  target_include_directories(mygrad INTERFACE include/)
  target_compile_features(mygrad INTERFACE cxx_std_20)

  add_executable(main main.cpp)
  target_link_libraries(main PRIVATE mygrad)
  target_compile_options(main PRIVATE "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>")
endif()
