cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(dll_example LANGUAGES CXX)
add_subdirectory(mathcal)
add_executable(dll_example src/main.cpp)
target_include_directories(dll_example PUBLIC include)
target_link_libraries(dll_example PRIVATE mathcal)
target_compile_features(dll_example PUBLIC cxx_std_20)

add_custom_command(TARGET dll_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mathcal> $<TARGET_FILE_DIR:dll_example>
)

