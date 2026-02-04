#!/usr/bin/env bash

set -euo pipefail

TARGET="${LAB_TARGET_IP:?}"
OPERATOR="${LAB_OPERATOR_IP:?}"
USER="${LAB_USER:?}"
WHO="${1:?usage: deploy <target|operator> <binary>}"
OUT="${2:?usage: deploy <target|operator> <binary>}"

case "$WHO" in
  target)   IP="${LAB_TARGET_IP:?}" ;;
  operator) IP="${LAB_OPERATOR_IP:?}" ;;
  *)        echo "unknown: $WHO" >&2; exit 1 ;;
esac

scp "$OUT" "$USER@$IP:~/"
