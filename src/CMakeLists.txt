
add_library(lib lib.c)
target_link_libraries(lib m)
target_include_directories(lib PUBLIC .)

add_executable(minefields minefields.c)
target_link_libraries(minefields lib)
