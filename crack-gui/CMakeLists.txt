cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

# include(${CMAKE_SOURCE_DIR}/cmake/sanitier.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(sanitizer)
# include(copy_target)

project(crack-gui VERSION 1.0 LANGUAGES C CXX)

FILE(GLOB_RECURSE SOURCES app/*.cpp app/*.c) 
FILE(GLOB_RECURSE LIB_SOURCE  src/*.cpp src/*.c)
FILE(GLOB_RECURSE HEADER  include/*.h include/*.hpp)
source_group(headers FILES ${HEADER}) 
add_executable(crack-gui ${SOURCES})
add_library(imgui STATIC ${LIB_SOURCE})
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
target_include_directories(imgui PUBLIC include/imgui)
target_compile_features(imgui PUBLIC cxx_std_20)
set_target_properties(imgui PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(crack-gui PRIVATE imgui)
target_include_directories(crack-gui PRIVATE include)
target_compile_features(crack-gui PRIVATE cxx_std_20)
set_target_properties(crack-gui PROPERTIES CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZER "Enable sanitizer" OFF)
if(ENABLE_SANITIZER)
  add_sanitier(crack-gui)
endif()

# option(ENABLE_COPY_TARGET "Enable copy target" OFF)
# if(ENABLE_COPY_TARGET)
#   copy_target(crack-gui)
# endif()
