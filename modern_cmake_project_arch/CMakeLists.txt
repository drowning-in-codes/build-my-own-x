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
option(ENABLE_SANITIZE_LEAK "Enable leak sanitizer" OFF)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

option(USE_CPM "use cpm for package management" OFF)
option(USE_CONAN "use conan for package management" OFF)
option(USE_VCPKG "use vcpkg for package management" OFF)

option(ENABLE_LTO "Enable LTO" ON)
set(LIBRARY_NAME Library)
set(EXECUTABLE_NAME Executable)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(AddGitSubmodule)
include(Docs)
if(${ENABLE_SANITIZE_ADDR} OR ${ENABLE_SANITIZE_UNDEF})
  include(Sanitizers)
  add_sanitizer_flags()
endif()
if(${ENABLE_TESTING} AND PROJECT_IS_TOP_LEVEL)
  include(CTest)
endif()
if(ENABLE_COVERAGE)
  message(STATUS "Enabling code coverage")
  include(CodeCoverage)
  append_coverage_compiler_flags()
endif()
if(ENABLE_LTO)
  include(LTO)
endif()

if(${ENABLE_WARNINGS})
  include(Warnings)
endif()

add_git_submodule(external/json)
if(USE_CPM)
  message(STATUS "Using CPM")
  include(CPM)
  CPMAddPackage("gh:fmtlib/fmt#7.1.3")
  CPMAddPackage("gh:nlohmann/json@3.10.5")
  CPMAddPackage("gh:catchorg/Catch2@3.4.0")
elseif(USE_CONAN)
  message(STATUS "Using Conan")
elseif(USE_VCPKG)
  message(STATUS "Using VCPKG")
else()
  message(STATUS "Using FetchContent")
  include(FetchContent)
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
endif()
add_subdirectory(configured)
add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(tests)

if (COMPILE_EXECUTABLE)
  add_subdirectory(app)
else()
  message("W/o exe. compiling")
endif()

include(GNUInstallDirs)
set(PROJECT_INSTALL_CMAKE_DIR "cmake")
install(TARGETS ${EXECUTABLE_NAME} 
      EXPORT "${PROJECT_NAME}_export"
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )


install(EXPORT "${PROJECT_NAME}_export"
        FILE "${PROJECT_NAME}Config.cmake"
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION "${PROJECT_INSTALL_CMAKE_DIR}"
    )


include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion 
)

install(FILES
"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${PROJECT_INSTALL_CMAKE_DIR}" 
)

set(CPACK_PACKAGE_VENDOR "proanimer")
set(CPACK_GENERATOR "ZIP;TZ")
set(CPACK_THREADS 0)
include(CPack)
