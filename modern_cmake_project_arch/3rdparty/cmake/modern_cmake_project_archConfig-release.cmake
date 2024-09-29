#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "modern_cmake_project_arch::Executable" for configuration "Release"
set_property(TARGET modern_cmake_project_arch::Executable APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(modern_cmake_project_arch::Executable PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Executable.exe"
  )

list(APPEND _cmake_import_check_targets modern_cmake_project_arch::Executable )
list(APPEND _cmake_import_check_files_for_modern_cmake_project_arch::Executable "${_IMPORT_PREFIX}/bin/Executable.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
