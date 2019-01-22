cmake_minimum_required(VERSION 3.13)
project(lasso)

set(CMAKE_CXX_STANDARD 17)

add_executable(test test.cpp lasso.cpp)

target_compile_options(test PRIVATE -O0)
target_link_libraries(test PRIVATE sfml-graphics sfml-window sfml-system)