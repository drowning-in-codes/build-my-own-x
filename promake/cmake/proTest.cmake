message(STATUSS "include proTest")

function(proTest testUnit)
  if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
  endif()
  add_test(NAME simpleTest COMMAND ${testUnit})
endfunction()


