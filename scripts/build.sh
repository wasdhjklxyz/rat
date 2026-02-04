#!/usr/bin/env bash

set -euo pipefail

NAME="${LAB_DOCKER_IMG_NAME:?not in devshell}"

BUILD_TYPE=""
case "${1:-}" in
  debug)      BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug" ;;
  release|"") BUILD_TYPE="-DCMAKE_BUILD_TYPE=Release" ;;
  *)          echo "Usage: build [debug|release]" >&2; exit 1 ;;
esac

docker run --rm -v "$(pwd)":/src -w /src "$NAME" \
  sh -c "cmake -B build $BUILD_TYPE && cmake --build build"
