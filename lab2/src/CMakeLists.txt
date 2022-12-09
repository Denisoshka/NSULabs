cmake_minimum_required(VERSION 3.23)

add_compile_options("-Wall")
add_compile_options("-Werror")
add_compile_options("-Wextra")

project(lab2 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab2 main.c)
