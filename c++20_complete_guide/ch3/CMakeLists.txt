file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp )
file(GLOB HEADERS CONFIGURE_DEPENDS include/*.hpp)
source_group("Header Files" FILES ${HEADERS})
add_executable(ch3 ${SOURCES} ${HEADERS})
target_include_directories(ch3 PRIVATE include)
