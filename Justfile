tests:
  mkdir -p tests-out
  cmake -S . -B tests-out -DCMAKE_BUILD_TYPE=Debug -DRUN_TESTS=ON
  cmake --build tests-out
  ./tests-out/InfinityCastle/InfinityCastle


lsp:
  ln -s ./build/compile_commands.json .

dev:
  mkdir -p build
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

build:
  cmake --build build

run: build
  cd src && ../build/InfinityCastle/InfinityCastle

release:
  cmake -S . -B build
  cmake --build build

clean:
  rm -rf build tests-out
