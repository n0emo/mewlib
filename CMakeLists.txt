cmake_minimum_required(VERSION 3.1...3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mew)

add_library(mew STATIC ./src/all.c)

target_include_directories(mew PUBLIC ./include/)

macro(define_example name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} mew)
endmacro()

define_example(html-basic "examples/html/basic.c")

