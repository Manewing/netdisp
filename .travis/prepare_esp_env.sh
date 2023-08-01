#!/bin/bash

set -eu -o pipefail

ESP_IDF_DIR="$HOME/esp/esp-idf"

if ! [ -d "$ESP_IDF_DIR" ]; then
  mkdir -p $(dirname "$ESP_IDF_DIR")
  git clone --recursive https://github.com/espressif/esp-idf.git "$ESP_IDF_DIR"
fi

pushd "$ESP_IDF_DIR"
git checkout v5.1
git submodule update --init --recursive
git checkout .

# Workaround see https://github.com/espressif/esp-idf/issues/6262
echo 'python-socketio<5; python_version<="2.7"' >> requirements.txt
popd

# Run the install script for ESP IDF, nop if everything is already installed
"$ESP_IDF_DIR/install.sh"
