cmake_minimum_required(VERSION 3.25)
#
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
#
project(ThreadPool)
file(GLOB SOURCES "src/*.cpp")

add_executable(ThreadPool ${SOURCES})
target_include_directories(ThreadPool PRIVATE "include/" )
set_target_properties(ThreadPool PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(ThreadPool PRIVATE -Wall -Wpedantic)
