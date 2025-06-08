clean:
    rm -rf build

configure-gcc:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=gcc

configure-clang:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=clang

build:
    cmake --build build

valgrind name: build
    valgrind --error-exitcode=1 './build/{{ name }}'

clean-build-gcc: clean configure-gcc build
clean-build-clang: clean configure-clang build
