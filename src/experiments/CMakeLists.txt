add_library(experiments experiments.c experiments.h)

target_include_directories(experiments PUBLIC .)

target_link_libraries(experiments minefield_algorithms)