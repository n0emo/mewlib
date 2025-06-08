add_executable(vector_test_exe ./containers/vector.c)

set(CMAKE_BUILD_TYPE "Debug")


target_link_libraries(vector_test_exe PRIVATE mew)

add_test(
    NAME vector_test
    COMMAND vector_test_exe
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
