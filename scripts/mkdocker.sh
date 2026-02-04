#!/usr/bin/env bash

set -euo pipefail

IMG="${LAB_DOCKER_IMG:?not in devshell}"
NAME="${LAB_DOCKER_IMG_NAME:?not in devshell}"

if ! docker image inspect "$NAME" &>/dev/null; then
  docker run --name "$NAME-tmp" -t "$IMG" dnf install -y gcc gcc-c++ cmake
  docker commit "$NAME-tmp" "$NAME"
  docker rm "$NAME-tmp"
fi
