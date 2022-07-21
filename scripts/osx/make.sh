#!/bin/bash
mkdir -p build
cd build
cmake -GNinja -DBUILD_BITCOIN_WALLET=OFF -DBUILD_BITCOIN_QT=OFF ..
ninja

