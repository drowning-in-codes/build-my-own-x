file(GLOB_RECURSE ALL_SOURCE_FILES
*.c *.h *.cpp *.hpp *.cxx *.hxx *.cc *.hh *.cppm *.ipp *.ixx)

add_custorm_target(format COMMAND clang-format -i ${ALL_SOURCE_FILES})

#  cmake -B build
# cmake --build build --target format
