cmake_minimum_required(VERSION 3.1...3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mew)

include(CTest)
enable_testing()

add_subdirectory(tests)

add_compile_options(
    -g

    -Wall
    -Wextra
    -Werror
    # TODO: Uncomment pedantic flag when the time comes
    # -pedantic
    -Wshadow
    -Wimplicit
    -Wconversion
    -Wundef
    -Wwrite-strings
    -Wformat=2
    -Wformat-security
    -Warray-bounds
    -Wnull-dereference
    -fstack-protector-strong

    -Wstrict-prototypes
    -Wmissing-prototypes

    $<$<NOT:$<CONFIG:Debug>>:-O2>
    $<$<NOT:$<CONFIG:Debug>>:-D_FORTIFY_SOURCE=2>
)

add_library(mew STATIC ./src/all.c)

target_compile_options(mew PRIVATE $<$<CONFIG:Debug>:--coverage>)
target_link_options(mew PRIVATE $<$<CONFIG:Debug>:--coverage>)
target_link_libraries(mew PRIVATE $<$<CONFIG:Debug>:gcov>)

target_include_directories(mew PUBLIC ./include/)

macro(define_example name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} mew)
endmacro()

define_example(html-basic "examples/html/basic.c")
define_example(html-controlflow "examples/html/controlflow.c")
define_example(html-attributes "examples/html/attributes.c")

define_example(containers-vector "examples/containers/vector.c")

