cmake_minimum_required(VERSION 3.1...3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mew)

include(CTest)
enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)

set(CMAKE_C_STANDARD 11)

add_compile_options(-DLOG_WITH_FILE)

if(MSVC)
    add_compile_options(/W3)
else()
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
    )
endif()

add_library(mew STATIC ./src/all.c)

target_include_directories(mew PUBLIC ./include/)
