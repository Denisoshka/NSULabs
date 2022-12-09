cmake_minimum_required(VERSION 3.23)

add_compile_options("-Wall")
add_compile_options("-Werror")
add_compile_options("-Wextra")

project(lab4 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab4 main.c)
