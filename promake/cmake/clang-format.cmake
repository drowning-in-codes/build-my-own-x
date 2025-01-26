file(GLOB_RECURSE ALL_SOURCE_FILES *.c *.cpp *.cc *.h *.hpp *.cxx *.hxx *.hh *.cppm *.ipp *.ixx)
add_custom_target(format COMMAND clang-format -i ${ALL_SOURCE_FILES})
