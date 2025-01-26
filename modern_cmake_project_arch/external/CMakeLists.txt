
# Add the external libraries which without cmake files
set(LOG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/log/src/log.cpp")
set(LOG_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/log/src/log.h")
add_library(Log STATIC ${LOG_SOURCES} ${LOG_HEADERS})
set_target_properties(Log PROPERTIES FOLDER "external")
