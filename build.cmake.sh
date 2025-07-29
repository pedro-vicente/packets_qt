#!/bin/bash
mkdir -p build
pushd build

if [[ "$OSTYPE" == "msys" ]]; then

cmake .. --fresh -DCMAKE_PREFIX_PATH="C:/Qt/6.8.2/msvc2022_64" 

elif [[ "$OSTYPE" == "darwin"* ]]; then

cmake .. --fresh -DCMAKE_PREFIX_PATH="/Users/pvn/Qt/6.8.2/macos" 

elif [[ "$OSTYPE" == "linux-gnu"* ]]; then

cmake .. --fresh -DCMAKE_FIND_DEBUG_MODE=ON -DQT_DEBUG_FIND_PACKAGE=ON

fi

cmake --build .
popd
