message(STATUS "include proDownload.cmake")

function(pro_IsNeedDownload rst filename hash_type hash)
  if(EXISTS ${filename})
    file(${hash_type} ${filename} origFileHash)
    string(TOLOWER ${hash} lhash)
    string(TOLOWER ${origFileHash} lOrigFileHash)
    if(${lhash} STREQUAL ${lOirgFileHash})
      set(${rst} "False" PARENT_SCOPE)
      return()
    endif()
  endif()
  set(${rst} "TRUE" PARENT_SCOPE)
endfunction()


function(pro_DownloadFile url filename hash_type hash)
  pro_IsNeedDownload(need ${filename} ${hash_type} ${hash})
  if(NOT need)
    message(STATUS "Found File:${filename}")
    return()
  endif()
  string(REGEX MATCH ".*/" dir ${filename})
  file(MAKE_DIRECTORY ${dir})
  message(STATUS "Downloading File")
  message(STATUS "- url  : ${url}")
  message(STATUS "- file name:${filename}")
  file(DOWNLOAD ${url} ${filename} SHOW_PROGRESS EXPECTED_HASH ${hash_type}=${hash} TLS_VERIFY ON)
endfunction()

function(pro_DownloadZip url zipname hash_type hash)
  set(filename "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${zipname}")
  pro_IsNeedDownload(need ${filename} ${hashtype} ${hash})
  if(NOT need)
    message(STATUS "Found File:${filename}")
    return()
  endif()
  pro_DownloadFile(${url} ${filename} ${hash_type} ${hash})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${filename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(pro_DownloadZip_Pro url zipname dir hash_type hash)
  set(filename "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${zipname}")
  pro_IsNeedDownload(need ${filename} ${hash_type} ${hash})
  if(NOT need)
    message(STATUS "Found File:${filename}")
    return()
  endif()
  pro_DownloadFile(${url} ${filename} ${hash_type} ${hash})
  file(MAKE_DIRECTORY ${dir})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${filename} WORKING_DIRECTORY ${dir})
endfunction()



function(pro_DownloadTestFile url filename hash_type hash)
  if(NOT ${pro_BuildTest_${PROJECT_NAME}})
    return()
  endif()
  pro_DownloadFile(${url} ${filename} ${hash_type} ${hash})
endfunction()

function(pro_DownloadTestZip url zipname hash_type hash)
  if(NOT ${pro_BuildTest_${PROJECT_NAME}})
    return()
  endif()
  pro_DownloadZip(${url} ${zipname} ${hash_type} ${hash})
endfunction()


function(pro_DownloadTestZip_Pro url zipname dir hash_type hash)
  if(NOT ${pro_BuildTest_${PROJECT_NAME}})
    return()
  endif()
  pro_DownloadZip_Pro(${url} ${zipname} ${dir} ${hash_type} ${hash})
endfunction()









