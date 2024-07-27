function(pro_BuildDoc doxyfilein)
  if(TARGET Doxygen::doxygen)
    set(doxyfileout ${CMAKE_CURRENT_BINARY_DIR}/${doxyfilein})
    configure_file(${doxyfilein} ${doxyfileout} @ONLY)
    set(targetName "${CMAKE_PROJECT_NAME}_doc")
    add_custom_target(${targetName} COMMAND Doxygen::doxygen ${doxyfileout}
                                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                                  COMMENT "Generating API documentation with Doxygen" VERBATIM
                              SOURCES ${doxyfilein} ${doxyfileout})

    set_target_properties(${targetName} PROPERTIES FOLDER ${CMAKE_PROJECT_NAME})
    source_group(doxyfile_input FILES ${doxyfilein})
    source_group(doxyfile_output FILES ${doxyfileout})
  else()
    message(STATUS "not have doxygen, ignore")
  endif()
endfunction()


