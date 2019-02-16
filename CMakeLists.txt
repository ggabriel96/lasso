cmake_minimum_required(VERSION 3.13)
project(lasso)

set(CMAKE_CXX_STANDARD 20)

include_directories( /usr/local/include )

add_executable(nested example/nested.cpp)

target_compile_options(nested PRIVATE -O0 -fconcepts)
target_link_libraries(nested PRIVATE sfml-graphics sfml-window sfml-system)