cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(proMake
  VERSON 1.0
  DESCRIPTION "A cmake project for builing"
)
message(STATUS "[Project] ${PROJECT_NAME} ${PROJECT_VERSION}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
inlcude(proInit)
option(ENABLE_SANITIZE_ADDR "Enable sanitizer" ON)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitizer" ON)
if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_UNDEF)
  # 引入库 使用函数  
  include(sanitizer)
  add_sanitizer_flags()
endif()
pro_InitProject(CXX_STANDARD 20)
pro_AddSubDirsRec(src)

pro_Export(
 DIRECTORIS
  "cmake"
)
