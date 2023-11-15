
add_executable(test test.c)
target_link_libraries(test lib m)

add_test(NAME test COMMAND test)
