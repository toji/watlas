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
  # Create output folder
  mkdir -p dist

  # Compile C/C++ code
  em++ \
    ${OPTIMIZE} \
    -std=c++17 \
    -DXA_MULTITHREADED=0 \
    -DNDEBUG \
    --bind \
    --no-entry \
    -s WASM=1 \
    -s STRICT=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MALLOC=emmalloc \
    -s EXPORT_ES6=1 \
    -s MODULARIZE=1 \
    -s ENVIRONMENT='web,worker,node' \
    -s ASSERTIONS=1 \
    -s NO_DISABLE_EXCEPTION_CATCHING \
    --emit-tsd watlas.d.ts \
    --extern-post-js './web/module-post.js' \
    -o ./dist/watlas.js \
    web/*.cpp \
    source/xatlas/xatlas.cpp

  #

  # Move artifacts
  #mv watlas-web.{js,wasm} dist
)
echo "============================================="
echo "Compiling wasm bindings done"
echo "============================================="