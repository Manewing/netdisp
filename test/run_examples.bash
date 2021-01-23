#!/bin/bash

set -eu -o pipefail

SCRIPT_DIR="$(dirname "$0")"
REPOROOT="$(realpath "$SCRIPT_DIR"/..)"
EXAMPLES_DIR="$REPOROOT/examples"
IMAGE_DIR="$REPOROOT/imgs"

if [ $# -ne 2 ]; then
  echo "usage: $0 <ip> <port>" >&2
  exit 1
fi

IP=$1
PORT=$2

echo "# Show Text View 4"
"$EXAMPLES_DIR/hello_world/show_text.bash" $IP $PORT "\n~View 4" 4
sleep 2

echo "# Show Text View 0"
"$EXAMPLES_DIR/hello_world/show_text.bash" $IP $PORT "\n~View 0" 0
sleep 2

echo "# Hello World"
"$EXAMPLES_DIR/hello_world/hello_world.bash" $IP $PORT
sleep 2

echo "# Simple Format"
"$EXAMPLES_DIR/formatting/simple_format.bash" $IP $PORT
sleep 2

echo "# System Monitor"
"$EXAMPLES_DIR/system_monitors/system_load.py" $IP $PORT --once --period 0.5
sleep 2

echo "# Show Image"
"$EXAMPLES_DIR/bitmaps/show_image.py" $IP $PORT "$IMAGE_DIR/droplet_icon.png"
sleep 2

echo "# Simple Composite"
"$EXAMPLES_DIR/gui/simple_composite.py" $IP $PORT
sleep 2

echo "# CryptoTracker"
"$EXAMPLES_DIR/gui/cryptotracker/cryptotracker.py" $IP $PORT euro bitcoin -p 0.5 --once
sleep 2
