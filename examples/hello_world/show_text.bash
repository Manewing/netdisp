#!/bin/bash

MAGIC=$(echo -en "\xef\xfa" | base64)

set -eu -o pipefail

function send_cmd() {
  echo -en "$MAGIC$1" | base64 -d | nc -b -w 0 -u $2 $3
}

function select_view() {
  echo -en "\x00\x$(printf "%x" $1)" | base64
}

function show_view() {
  echo -en "\x01\x$(printf "%x" $1)" | base64
}

function show_text() {
  LEN=$(echo -en "$1" | wc -c)
  echo -en "\x03\x00\x$(printf "%x" $LEN)""$1" | base64
}

if [ $# -ne 4 ]; then
  echo "usage: $0 <ip> <port> <text> <view>" >&2
  exit 1
fi

IP=$1
PORT=$2
TXT=$3
VIEW=$4

send_cmd "$(show_view $VIEW)$(select_view $VIEW)$(show_text "$TXT")" $IP $PORT