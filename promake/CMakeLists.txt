cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(proMake
  VERSON 1.0
  DESCRIPTION "A cmake project for builing"
)
message(STATUS "[Project] ${PROJECT_NAME} ${PROJECT_VERSION}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
inlcude(proInit)

pro_InitProject(CXX_STANDARD 20)
pro_AddSubDirsRec(src)

pro_Export(
 DIRECTORIS
  "cmake"
)
