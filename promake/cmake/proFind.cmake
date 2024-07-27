message(STATUS "include pro")


function (proFind libname)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

  find_library(
  ${libname}_LIBRARY
  NAMES ${libname}
  HINTS ${PROJECT_SOURCE_DIR}/dep/
  PATH_SUFFIXES lib bin build/Release build/Debug
)

  find_path(
    ${libname}_INCLUDE_DIR
    NAMES ${libname}/${libname}.hpp
    HINTS ${PROJECT_SOURCE_DIR}/dep/include/
  )

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(${libname} DEFAULT_MSG ${libname}_LIBRARY ${libname}_INCLUDE_DIR)
  mark_as_advanced(${libname}_LIBRARY ${libname}_INCLUDE_DIR) # 高级变量
  if(NOT TARGET ${libname}::${libname})
    add_library(${libname}::${libname} UNKNOWN IMPORTED)
    #  因为还不能确定它是
    # 静态库还是动态库，所以类型为 UNKNOWN，并设置了 IMPORTED 标志。
    set_target_properties(${libname}::${libname} PROPERTIES
    IMPORTED_LOCATION ${${libname}_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${${libname}_INCLUDE_DIR}
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
  )
  endif()
endfunction()


