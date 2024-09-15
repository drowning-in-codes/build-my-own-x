function (copy_target target_name )
  if(target_name STREQUAL "")
    return()
  endif()
  message(STATUS "Copying target ${target_name}")
foreach(lib ${dylib_target})

endforeach()
    add_custom_command(
        TARGET ${target_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target_name}>  $<TARGET_RUNTIME_DLLS:${target_name}> ${CMAKE_SOURCE_DIR}
    )
endfunction()
