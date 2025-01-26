function (add_sanitier target)
message(STATUS "Adding sanitizer to target ${target}")
  if (CMAKE_CXX_COMPILER_ID MATCHES "CLANG" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-fno-omit-frame-pointer")
    add_link_options("-fno-omit-frame-pointer")
    target_compile_options(${target} PRIVATE -fsanitize=address)
    target_link_libraries(${target} PRIVATE -fsanitize=address)
    target_compile_options(${target} PRIVATE -fsanitize=undefined)
    target_link_libraries(${target} PRIVATE -fsanitize=undefined)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(${target} PRIVATE /fsanitize=address)
else()
    message(WARNING "Sanitier is not supported for ${CMAKE_CXX_COMPILER_ID}")
  endif()
endfunction()
