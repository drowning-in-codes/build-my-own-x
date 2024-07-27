macro(pro_InitproMake)
  include(FetchContent)
  set(proMake_VERSION 0.6.0)
  message(STATUS "find package: proMake ${proMake_VERSION}")
  find_package(proMake ${proMake_VERSION} QUIET)
  if(NOT proMake_FOUND)
    message(STATUS "proMake ${proMake_VERSION} not found")
    set(_address "https://github.com/drowning-in-codes/proMake")
    message(STATUS "fetch: ${_address} with tag ${proMake_VERSION}")
    FetchContent_Declare(
      proMake
      GIT_REPOSITORY ${_address}
      GIT_TAG ${proMake_VERSION}
    )
    FetchContent_MakeAvailable(proMake)
    message(STATUS "proMake ${proMake_VERSION} build done")
  endif()
endmacro()
