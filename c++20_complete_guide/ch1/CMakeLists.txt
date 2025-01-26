file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")
add_executable(ch1 ${SOURCES} ${HEADERS})
target_include_directories(ch1 PRIVATE "include/")
