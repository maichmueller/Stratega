cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.h")
add_executable (GoogleTest ${sources})

target_include_directories(GoogleTest PUBLIC include)
target_link_libraries(GoogleTest PUBLIC Stratega)