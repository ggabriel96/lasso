cmake_minimum_required(VERSION 3.13)
project(lasso)

set(CMAKE_CXX_STANDARD 20)

add_executable(test example/test.cpp)

target_compile_options(test PRIVATE -O0 -fconcepts)
target_link_libraries(test PRIVATE sfml-graphics sfml-window sfml-system)