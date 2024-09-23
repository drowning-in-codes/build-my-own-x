set(LIBRARY_SOURCES "my_lib.cpp")
set(LIBRARY_HEADERS "my_lib.h")

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

target_include_directories(${LIBRARY_NAME} PUBLIC "./"
                            "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${LIBRARY_NAME} PUBLIC cxxopts::cxxopts nlohmann_json::nlohmann_json spdlog::spdlog fmt::fmt)
if(${ENABLE_WARNINGS})
  target_set_warnings(TARGET ${LIBRARY_NAME} ENABLE ${ENABLE_WARNINGS} ENABLE_AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()
