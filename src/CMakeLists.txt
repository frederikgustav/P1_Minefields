add_library(minefield_algorithms minefield_algorithms.c)
target_link_libraries(minefield_algorithms m)
target_include_directories(minefield_algorithms PUBLIC .)

add_library(minefields minefields.c)
target_link_libraries(minefields m)
target_include_directories(minefields PUBLIC .)

add_executable(main main.c)
target_link_libraries(main minefields)
target_link_libraries(main minefield_algorithms)
