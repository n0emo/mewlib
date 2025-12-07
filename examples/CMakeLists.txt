macro(define_example name src)
    set(CMAKE_C_STANDARD 11)
    add_executable(${name} ${src})
    target_link_libraries(${name} mew)
endmacro()

define_example(html_basic "html/basic.c")
define_example(html_controlflow "html/controlflow.c")
define_example(html_attributes "html/attributes.c")

define_example(containers_vector "containers/vector.c")
define_example(containers_assocmap "containers/assocmap.c")
