project(Example-task)

# Lib
add_library(example_foo_lib foo.c)
target_include_directories(example_foo_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main
add_executable(example_main main.c)
target_link_libraries(example_main PRIVATE example_foo_lib)

# Tests
enable_testing()
add_executable(test.c)
target_link_libraries(<target> ... <item>... ...)
