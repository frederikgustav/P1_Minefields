add_executable(test_minefields test_minefields.c)
target_link_libraries(test_minefields minefields m)
target_link_libraries(test_minefields minefield_algorithms m)

add_test(NAME test_minefields COMMAND test)
