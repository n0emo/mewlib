set(CMAKE_BUILD_TYPE "Debug")

macro(create_test name src)
    add_executable("${name}_test_exe" ${src})
    set(CMAKE_C_STANDARD 11)
    target_link_libraries("${name}_test_exe" PRIVATE mew)
    target_link_options("${name}_test_exe" PRIVATE -fsanitize=address)
    add_test(
        NAME "${name}_test"
        COMMAND "${name}_test_exe"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endmacro()

create_test(containers_vector ./containers/vector.c)
create_test(core_strings_cstr ./core/strings/cstr.c)
create_test(core_strings_sb ./core/strings/sb.c)
create_test(containers_hashmap ./containers/map/hashmap.c)
create_test(containers_assocmap ./containers/map/assoc.c)
