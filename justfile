clean:
    rm -rf build

[working-directory: '.']
configure-clang:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=clang

[working-directory: '.']
configure-gcc:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=gcc

[working-directory: '.']
configure-gcc-debug:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug

[working-directory: 'build']
build:
    cmake --build .

[working-directory: 'build']
test: build
    ctest --output-on-failure

[working-directory: 'build']
coverage: clean configure-gcc-debug test
    ctest -T Coverage
    find . -name '*.gcda' | xargs gcov
    lcov --directory . --capture --output-file coverage.info
    genhtml -o coverage coverage.info

[working-directory: '.']
coverage-open: coverage
    xdg-open ./build/coverage/index.html

valgrind name: build
    valgrind --error-exitcode=1 './build/{{ name }}'

clean-build-gcc: clean configure-gcc build
clean-build-clang: clean configure-clang build
