add_library(minefield_algorithms minefield_algorithms.c)
target_link_libraries(minefield_algorithms m)
target_include_directories(minefield_algorithms PUBLIC .)

add_library(experiments experiments.c)
target_link_libraries(experiments m)
target_include_directories(experiments PUBLIC .)

add_library(minefields minefields.c)
target_link_libraries(minefields m)
target_include_directories(minefields PUBLIC .)

add_executable(main main.c)
target_link_libraries(main minefields)
target_link_libraries(experiments minefields)
target_link_libraries(experiments minefield_algorithms)
target_link_libraries(main minefield_algorithms)
target_link_libraries(main experiments)