#!/bin/bash

set -eu -o pipefail

ESP_IDF_DIR="$HOME/esp/esp-idf"

if ! [ -d "$ESP_IDF_DIR" ]; then
  mkdir -p $(dirname "$ESP_IDF_DIR")
  git clone --recursive https://github.com/espressif/esp-idf.git "$ESP_IDF_DIR"
fi

pushd "$ESP_IDF_DIR"
git checkout v4.2
git submodule update --init --recursive
popd

# Run the install script for ESP IDF, nop if everything is already installed
"$ESP_IDF_DIR/install.sh"
