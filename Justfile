dev:
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

build:
  cmake --build build

run: build
  cd src && ../build/InfinityCastle/InfinityCastle

release:
  cmake -S . -B build
  cmake --build build

clean:
  rm -rf build
