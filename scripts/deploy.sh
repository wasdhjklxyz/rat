#!/usr/bin/env bash

set -euo pipefail

TARGET="${LAB_TARGET_IP:?}"
OPERATOR="${LAB_OPERATOR_IP:?}"
USER="${LAB_USER:?}"
SSH_KEY="${LAB_SSH_KEY:?}"

deploy() {
  local out="build/src/$1/$1"
  local scp_cmd=(scp)
  [[ -f "$SSH_KEY" ]] && scp_cmd+=(-i "$SSH_KEY")
  [[ -f "$out" ]] && "${scp_cmd[@]}" "$out" "$USER@$2:~/"
  [[ -f "${out}_debug" ]] && "${scp_cmd[@]}" "${out}_debug" "$USER@$2:~/"
  return 0
}

deploy "agent" "$TARGET"
deploy "c2" "$OPERATOR"
