#!/bin/bash

set -eu -o pipefail

echo "This script will create a systemd user service which runs NetDisp CryptoTracker"

if [ $# -ne 5 ]; then
  echo "usage: $0 <netdisp-ip> <netdisp-port> <netdisp-view> <currency> <crypto>" >&2
  exit 1
fi

CONF_PATH="$HOME/.config/systemd/user"
SERVICE_NAME="netdisp_cryptotracker_$3" # Encode view in service name
UNIT_DESC_PATH="$CONF_PATH/$SERVICE_NAME.service"

echo -n "Installing to $UNIT_DESC_PATH, ok (y/n)?: "
read CONTINUE
if [[ $CONTINUE != y && $CONTINUE != yes ]]; then
  echo "aborted"
  exit 1
fi

mkdir -p $CONF_PATH

CRYPTOTRACKER_SCRIPT="$PWD/cryptotracker.py"

echo "testing..."
EXEC_START="/usr/bin/python3 $CRYPTOTRACKER_SCRIPT $1 $2 -v $3 $4 $5 -p 0.5"
$EXEC_START --once -p 0.05

cat > $UNIT_DESC_PATH <<EOF
[Unit]
Description=NetDisp CryptoTracker Python Service

[Service]
ExecStart=$EXEC_START
EOF

systemctl --user list-unit-files | grep $SERVICE_NAME
systemctl --user start $SERVICE_NAME
systemctl --user status $SERVICE_NAME
