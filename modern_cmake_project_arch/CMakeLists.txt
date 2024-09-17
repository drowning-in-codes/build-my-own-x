cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(modern_cmake_project_arch VERSION 1.0.0 LANGUAGES C CXX)

option(ENABLE_TESTING "Enable a Unit Testing Build" ON)
option(COMPILE_EXECUTABLE "Whether to compile the executable" ON)

option(ENABLE_WARNINGS "Enable warnings" ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable warnings as erros" ON)

option(ENABLE_SANITIZE_ADDR "Enable address sanitizer" OFF)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitizer" OFF)

set(LIBRARY_NAME Library)
set(EXECUTABLE_NAME Executable)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(AddGitSubmodule)
include(FetchContent)
include(Docs)
if(${ENABLE_SANITIZE_ADDR} OR ${ENABLE_SANITIZE_UNDEF})
  include(Sanitizers)
  add_sanitizer_flags()
endif()
if(${ENABLE_TESTING} AND PROJECT_IS_TOP_LEVEL)
  include(CTest)
endif()

if(${ENABLE_WARNINGS})
  include(Warnings)
endif()

add_git_submodule(external/json)

FetchContent_Declare(fmt
GIT_REPOSITORY https://github.com/fmtlib/fmt
GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(spdlog
GIT_REPOSITORY https://github.com/gabime/spdlog
GIT_TAG v1.14.1
GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(cxxopts
GIT_REPOSITORY https://github.com/jarro2783/cxxopts
GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(cxxopts)

FetchContent_Declare(Catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2
GIT_TAG v3.7.1
GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(configured)
add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(tests)

if (COMPILE_EXECUTABLE)
  add_subdirectory(app)
else()
  message("W/o exe. compiling")
endif()
