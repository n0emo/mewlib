add_executable(vector_test_exe ./containers/vector.c)

target_link_libraries(vector_test_exe
    PRIVATE mew
)

add_test(
    NAME vector_test
    COMMAND vector_test_exe
)
