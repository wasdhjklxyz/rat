#!/usr/bin/env bash

set -euo pipefail

NAME="${LAB_DOCKER_IMG_NAME:?not in devshell}"

docker run --rm -v "$(pwd)":/src -w /src "$NAME" gcc "$@"
