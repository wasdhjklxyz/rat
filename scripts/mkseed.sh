#!/usr/bin/env bash

set -euo pipefail

DIR="${LAB_DISK_DIR:?not in devshell}"
NAME="${1:?usage: mkseed <target|operator>}"
USER="${LAB_USER:?}"
PASS="${LAB_PASS:?}"
GATEWAY="${LAB_GATEWAY:?}"
SSH_KEY="${LAB_SSH_KEY:?}"

case "$NAME" in
  target)   IP="${LAB_TARGET_IP:?}" ;;
  operator) IP="${LAB_OPERATOR_IP:?}" ;;
  *)        echo "unknown vm: $NAME" >&2; exit 1 ;;
esac

META="$DIR/$NAME-cloudinit"
rm -rf "$META"
mkdir -p "$META"

cat > "$META/meta-data" <<EOF
instance-id: $NAME
local-hostname: $NAME
EOF

HASH=$(openssl passwd -6 "$PASS")

if [[ ! -f "$SSH_KEY" ]]; then
  ssh-keygen -t ed25519 -f "$SSH_KEY" -N "" -q
fi
PUBKEY=$(cat "${SSH_KEY}.pub")

cat > "$META/user-data" <<EOF
#cloud-config
ssh_pwauth: true
users:
  - name: $USER
    sudo: ALL=(ALL) NOPASSWD:ALL
    shell: /bin/bash
    lock_passwd: false
    passwd: $HASH
EOF

[[ -n "$PUBKEY" ]] && cat >> "$META/user-data" <<EOF
    ssh_authorized_keys:
      - $PUBKEY
EOF

cat >> "$META/user-data" <<EOF

write_files:
  - path: /etc/NetworkManager/system-connections/lab.nmconnection
    permissions: '0600'
    content: |
      [connection]
      id=lab
      type=ethernet
      autoconnect=true
      [ipv4]
      method=manual
      addresses=$IP/24
      gateway=$GATEWAY
      [ipv6]
      method=disabled

runcmd:
  - nmcli con load /etc/NetworkManager/system-connections/lab.nmconnection
  - nmcli con up lab
  - echo "$USER ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USER
EOF

genisoimage -output "$DIR/$NAME-seed.iso" \
  -volid cidata -joliet -rock \
  "$META/user-data" "$META/meta-data" 2>/dev/null

echo "created $DIR/$NAME-seed.iso (user=$USER pass=$PASS ip=$IP)"
