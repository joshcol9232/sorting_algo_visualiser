add_executable(stat_array_test stat_array_test.cc)
target_link_libraries(stat_array_test PRIVATE ${TARGET_LIBRARIES})

enable_testing()

add_test(
  NAME stat_array_test
  COMMAND $<TARGET_FILE:stat_array_test>
)
