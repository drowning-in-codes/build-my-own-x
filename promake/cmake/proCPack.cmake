message(STATUS "include proCPack")


function (proCPack)
  set(CPACK_PACKAGE_NAME "pro")
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_THREAD 0)
  set(CPACK_ZIP_PACKAGE_MAINTEAINER  "pro")
  include(CPACK)

endfunction()
