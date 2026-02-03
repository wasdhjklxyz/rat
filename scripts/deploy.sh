#!/usr/bin/env bash

set -euo pipefail

TARGET="${LAB_TARGET_IP:?}"
OPERATOR="${LAB_OPERATOR_IP:?}"
USER="${LAB_USER:?}"
OUT="${1:?usage: deploy <target|operator> <binary>}"
WHO="${2:?usage: deploy <target|operator> <binary>}"

scp "$OUT" "$USER@$TARGET:~/"
