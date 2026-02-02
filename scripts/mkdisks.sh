#!/usr/bin/env bash

set -euo pipefail

DIR="${LAB_DISK_DIR:?not in devshell}"
CLOUD_IMG="$DIR/Rocky-10-GenericCloud-Base.latest.x86_64.qcow2"
CLOUD_URL="https://download.rockylinux.org/pub/rocky/10/images/x86_64/Rocky-10-GenericCloud-Base.latest.x86_64.qcow2"

confirm() { read -rp "$1 [y/N] " a; [[ ${a,,} == y ]]; }
mkdir -p "$DIR"

if [[ ! -f "$CLOUD_IMG" ]]; then
  echo "downloading rocky cloud image..."
  curl -L -o "$CLOUD_IMG" "$CLOUD_URL"
fi

for name in operator target; do
  disk="$DIR/$name.qcow2"
  if [[ -f "$disk" ]]; then
    confirm "$name disk exists. Nuke?" && rm -f "$disk" || continue
  fi
  qemu-img create -f qcow2 -b "$(basename $CLOUD_IMG)" -F qcow2 "$disk" 40G
  echo "created $disk"
done
