#!/usr/bin/env bash

set -euo pipefail

IFACE="${LAB_BRIDGE:?not in devshell}"
NETCIDR="${LAB_NETCIDR:?}"
GWCIDR="${LAB_GATEWAY:?}/24"

up() {
  sudo ip link add "$IFACE" type bridge
  sudo ip addr add "$GWCIDR" dev "$IFACE"
  sudo ip link set "$IFACE" up
  sudo ip tuntap add dev tap-target mode tap user "$USER"
  sudo ip link set tap-target master "$IFACE"
  sudo ip link set tap-target up
  sudo ip tuntap add dev tap-operator mode tap user "$USER"
  sudo ip link set tap-operator master "$IFACE"
  sudo ip link set tap-operator up
  echo "$IFACE up"
}

down() {
  sudo sysctl -qw net.ipv4.ip_forward=0 2>/dev/null || true
  sudo iptables -t nat -D POSTROUTING \
    -s "$NETCIDR" ! -d "$NETCIDR" -j MASQUERADE 2>/dev/null || true
  sudo ip link del tap-target 2>/dev/null || true
  sudo ip link del tap-operator 2>/dev/null || true
  sudo ip link set "$IFACE" down 2>/dev/null || true
  sudo ip link delete "$IFACE" 2>/dev/null || true
  echo "$IFACE down"
}

escape() {
  sudo sysctl -qw net.ipv4.ip_forward=1
  sudo iptables -t nat -A POSTROUTING \
    -s "$NETCIDR" ! -d "$NETCIDR" -j MASQUERADE
  echo "$IFACE escaped"
}

jail() {
  sudo sysctl -qw net.ipv4.ip_forward=0
  sudo iptables -t nat -D POSTROUTING \
    -s "$NETCIDR" ! -d "$NETCIDR" -j MASQUERADE 2>/dev/null || true
  echo "$IFACE jailed"
}

case "${1:-}" in
  up)     up ;;
  down)   down ;;
  escape) escape ;;
  jail)   jail ;;
  *)      echo "Usage: net {up|down|escape|jail}" >&2; exit 1 ;;
esac
