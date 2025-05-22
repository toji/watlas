#!/bin/bash

set -e

export OPTIMIZE="-Os"
export LDFLAGS="${OPTIMIZE}"
export CFLAGS="${OPTIMIZE}"
export CPPFLAGS="${OPTIMIZE}"

echo "============================================="
echo "Compiling wasm bindings"
echo "============================================="
(
  # Compile C/C++ code
  em++ \
    -std=c++17 \
    -DXA_MULTITHREADED=0 \
    -DNDEBUG \
    --bind \
    --no-entry \
    -s STRICT=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MALLOC=emmalloc \
    -s MODULARIZE=1 \
    -s ENVIRONMENT='web,worker' \
    -s ASSERTIONS=1 \
    -o ./xatlas-web.js \
    web/*.cpp \
    source/xatlas/xatlas.cpp

  # ${OPTIMIZE} \
  #--post-js './web/module-post.js' \

  # Create output folder
  mkdir -p dist
  # Move artifacts
  mv xatlas-web.{js,wasm} dist
)
echo "============================================="
echo "Compiling wasm bindings done"
echo "============================================="