set(CMAKE_BUILD_TYPE "Debug")

macro(create_test name src)
    add_executable("${name}_test_exe" ${src})
    target_link_libraries("${name}_test_exe" PRIVATE mew)
    add_test(
        NAME "${name}_test"
        COMMAND "${name}_test_exe"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endmacro()

create_test(vector ./containers/vector.c)
create_test(hashmap ./containers/hashmap.c)
