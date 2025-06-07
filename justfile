clean:
    rm -rf build

configure-gcc:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=gcc

configure-clang:
    cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=clang

build:
    cmake --build build

clean-build-gcc: clean configure-gcc build
clean-build-clang: clean configure-clang build
