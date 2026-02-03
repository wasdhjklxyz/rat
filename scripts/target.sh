#!/usr/bin/env bash

set -euo pipefail

BRIDGE="${LAB_BRIDGE:?not in devshell}"
DIR="${LAB_DISK_DIR:?}"
DISK="$DIR/target.qcow2"
SEED="$DIR/target-seed.iso"
MAC="${LAB_TARGET_MAC:?}"

ip link show "$BRIDGE" &>/dev/null || { echo "run: net up" >&2; exit 1; }
[[ -f "$DISK" ]] || { echo "run: mkdisks" >&2; exit 1; }
[[ -f "$SEED" ]] || { echo "run: mkseed target" >&2; exit 1; }

qemu-system-x86_64 \
  -machine q35 \
  -enable-kvm \
  -m 4G \
  -cpu host \
  -smp 4 \
  -drive file="$DISK",format=qcow2 \
  -cdrom "$SEED" \
  -netdev tap,id=net0,ifname=tap-target,script=no,downscript=no \
  -device virtio-net-pci,netdev=net0,mac="$MAC" \
  -vga virtio \
  -display gtk
