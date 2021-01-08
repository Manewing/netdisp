#!/bin/bash

set -eu -o pipefail

SCRIPTDIR="$(dirname "$0")"
REPOROOT="$(realpath "$SCRIPTDIR"/..)"

pushd "$REPOROOT/build_test"
ctest
popd