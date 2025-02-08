cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(mathcal LANGUAGES CXX)
add_library(mathcal SHARED src/mathcal.cpp)
target_include_directories(mathcal PUBLIC include)
target_compile_features(mathcal PUBLIC cxx_std_20)


