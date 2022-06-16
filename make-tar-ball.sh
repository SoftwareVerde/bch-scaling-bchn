#!/bin/bash

# build dependencies for static linking
cd depends
make && make build-linux64 || exit $?

# out of source build of BCHN
cd .. # return to base directory
mkdir -p build && cd build || exit $?
cmake -GNinja .. -DCMAKE_BUILD_TYPE=Release -DENABLE_MAN=OFF -DCMAKE_TOOLCHAIN_FILE=../cmake/platforms/Linux64.cmake || exit $?
ninja && ninja package || exit $?
