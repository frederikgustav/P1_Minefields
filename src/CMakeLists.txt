add_subdirectory(experiments)
add_subdirectory(minefield_algorithms)
add_subdirectory(minefields)

add_executable(main main.c)
target_link_libraries(main minefield_algorithms minefields experiments)