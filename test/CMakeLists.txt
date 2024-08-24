
enable_testing()

function(add_test_executable name)
  add_executable(${name} ${name}.cc)
  target_link_libraries(${name} PRIVATE ${TARGET_LIBRARIES})
  add_test(
    NAME ${name}
    COMMAND $<TARGET_FILE:${name}>
  )
endfunction()

add_test_executable(stat_array_test)
add_test_executable(sorting_algorithms_test)

