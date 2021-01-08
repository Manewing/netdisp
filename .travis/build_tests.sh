#!/bin/bash

set -eu -o pipefail

SCRIPTDIR="$(dirname "$0")"
REPOROOT="$(realpath "$SCRIPTDIR"/..)"

mkdir -p "$REPOROOT/build_test"
pushd "$REPOROOT/build_test"
cmake "$REPOROOT/test"
make
popd