cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

project(concurrency)

add_executable(concurrency src/main.cpp)
target_include_directories(concurrency PRIVATE include)
target_compile_features(concurrency PRIVATE cxx_std_20)

