#!/bin/bash

set -eu -o pipefail

if [ $# -ne 2 ]; then
  echo "usage: $0 <ip> <port>" >&2
  exit 1
fi

echo -n "Normal
*Bold*
_Italic_
~Centered
" | nc -b -w 0 -u $1 $2
