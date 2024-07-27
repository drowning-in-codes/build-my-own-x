message(STATUS "include proInit.cmake")

include("${CMAKE_CURRENT_LIST_DIR}/proBasic.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/proBuild.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/proDownload.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/proGit.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/proPackage.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/proQt.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/proDoc.cmake")

set(CPM_USE_LOCAL_PACKAGES TRUE CACHE BOOL "Use local packages")
include("${CMAKE_CURRENT_LIST_DIR}/CPM.cmake")

macro(pro_InitProject)
  # add postfix
  set(CMAKE_DEBUG_POSTFIX "d")
  set(CMAKE_RELEASE_POSTFIX "")
  set(CMAKE_MINSIZEREL_POSTFIX "msr")
  set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED  TRUE)
  # set default build type
  if(NOT CMAKE_BUILD_TYPE)
    message(NOTICE "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  endif()

  # add compile definitions
  add_compile_definitions(pro_CONFIG_$<UPPER_CASE:$<CONFIG>>)

  add_compile_definitions(
    $<$<CONFIG:DEBUG>:pro_CONFIG_POSTFIX="${CMAKE_DEBUG_POSTFIX}">
      $<$<CONFIG:Release>:pro_CONFIG_POSTFIX="">
      $<$<CONFIG:MinSizeRel>:pro_CONFIG_POSTFIX="${CMAKE_MINSIZEREL_POSTFIX}">
      $<$<CONFIG:RelWithDebInfo>:pro_CONFIG_POSTFIX="${CMAKE_RELWITHDEBINFO_POSTFIX}">
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>>:pro_CONFIG_POSTFIX="">)

  # check compiler version
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
    message(STATUS "Compiler:Clang ${CMAKE_CXX_COMPILER_VERSION}")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12.0)
      message(FATAL_ERROR "Clang version must be at least 12.0!")
      return()
    endif()
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Compiler:GCC ${CMAKE_CXX_COMPILER_VERSION}")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
      message(FATAL_ERROR "GCC version must be at least 10.0!")
      return()
    endif()
    elistif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message(STATUS "Compiler:MSVC ${MSVC_VERSION}")
    if(${CMAKE_COMPILER_VERSION} VERSION_LESS 19.28)
      message(FATAL_ERROR "MSVC version must be at least 19.28!")
      return()
    endif()
  else()
    message(FATAL_ERROR "Unsupported compiler:${CMAKE_CXX_COMPILER_ID}")
  endif()

  message(STATUS "CXX_STRANDARD: ${CMAKE_CXX_STANDARD}")
  # set default install path
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    pro_Path_Back(root ${CMAKE_INSTALL_PREFIX} 1) set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Default install path" FORCE)    pro_Path_Back(root ${CMAKE_INSTALL_PREFIX} 1) set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Default install path" FORCE)
  endif()

  set("pro_BuildTest_${PROJECT_NAME}" TRUE CACHE BOOL "build tests for ${PROJECT_NAME}")
  if(NOT pro_RootProjectPath)
    set(pro_RootProjectPath ${PROJECT_SOURCE_DIR})
  endif()

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${pro_RootProjectPath}/bin")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Ubpa_RootProjectPath}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${pro_RootProjectPath}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # build docs
  option(pro_${CMAKE_PROJECT_NAME}_BuildDoc "Build ${CMAKE_PROJECT_NAME} document" OFF)
  if(pro_${CMAKE_PROJECT_NAME}_BuildDoc)
    find_package(Doxygen REQUIRED)
  else()
    message(STATUS "don't build docs")
  endif()
endmacro()







