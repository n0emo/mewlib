set(CMAKE_BUILD_TYPE "Debug")

macro(create_test name src)
    add_executable("${name}_test_exe" ${src})
    target_include_directories("${name}_test_exe" PRIVATE "${CMAKE_SOURCE_DIR}")
    target_compile_options(
        "${name}_test_exe" PRIVATE

        -g

        -Wall
        -Wextra
        -pedantic
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
    )
    target_link_options(
        "${name}_test_exe" PRIVATE -fsanitize=address
    )

    add_test(
        NAME "${name}_test"
        COMMAND "${name}_test_exe"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endmacro()

create_test(containers-vector ./containers/vector.c)
create_test(core-strings-cstr ./core/strings/cstr.c)
create_test(core-strings-sb ./core/strings/sb.c)
create_test(containers-hashmap ./containers/hashmap.c)
