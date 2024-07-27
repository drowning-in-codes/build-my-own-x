message(STATUS "include proFetch")

function(proFetch depName depUrl depTag)

  # 获取字符串长度
  string(LENGTH "${depUrl}" REPO_URL_LENGTH)

  # 获取后缀 ".git" 的长度
  set(GIT_SUFFIX_LENGTH 4)

  # 获取字符串最后四个字符
  string(SUBSTRING "${depUrl}" ${REPO_URL_LENGTH} -${GIT_SUFFIX_LENGTH} REPO_URL_LAST_FOUR)

  # 检查最后四个字符是否为 ".git"
  if("${REPO_URL_LAST_FOUR}" STREQUAL ".git")
    message(STATUS "download from git")
  else()
    message(FATAL_ERROR "The repository URL does not end with .git, not support yet.")
    return()
  endif()
  include(FetchContent)
  FetchContent_Declare(
    ${depName}
    GIT_REPOSITORY ${depUrl}
    GIT_TAG  ${depTag}
  )
  FetchContent_MakeAvailable(${depName})

endfunction()
