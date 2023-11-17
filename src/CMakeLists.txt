
add_library(minelib minelib.c)
target_link_libraries(minelib m)
target_include_directories(minelib PUBLIC .)

add_executable(minefields minefields.c)
target_link_libraries(minefields minelib)
