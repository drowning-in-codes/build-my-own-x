message(STATUS "include UbpaGit.cmake")
macro(Ubpa_InitGit)
  message(STATUS "--------------------")
  find_package(Git REQUIRED)
  message(STATUS "GIT_FOUND: ${GIT_FOUND}" )
  message(STATUS "GIT_EXECUTABLE: ${GIT_EXECUTABLE}" )
  message(STATUS "GIT_VERSION_STRING: ${GIT_VERSION_STRING}" )

endmacro()

function(pro_UpdateSubModule)
  if(NOT GIT_FOUND)
    message(FATAL_ERROR "you should call pro_InitGit first")
  endif()
  execute_process(COMMMAND ${GIT_EXECUTABLE} submodule  init  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

  execute_process(COMMMAND ${GIT_EXECUTABLE} submodule  update  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

endfunction()

