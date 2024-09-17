function(add_git_submodule dir)
  find_package(Git REQUIRED)
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${dir}/CMakeLists.txt)
    # 如果不存在, 则根据.gitmodules中信息下载模块 
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init -- ${CMAKE_SOURCE_DIR}/${dir}
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()
  # 添加模块
  if(EXISTS ${CMAKE_SOURCE_DIR}/${dir}/CMakeLists.txt)
    message("Adding: ${dir}/CMakeLists.txt")
    add_subdirectory(${dir})
  else()
    message("Could not add: ${dir}/CMakeLists.txt")
  endif()
endfunction()
