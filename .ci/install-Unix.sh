#! /usr/bin/env bash

# Install bazel, valgrind in unix-like system

set -eu

if [ "$(uname)" == 'Darwin' ]; then
    /usr/bin/ruby -e "$(curl -fsSL \
    https://raw.githubusercontent.com/Homebrew/install/master/install)"
    brew tap bazelbuild/tap
    brew install bazelbuild/tap/bazel
elif [ "$(uname)" == 'Linux' ]; then
    # Ubuntu in fact
    echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | \
        sudo tee /etc/apt/sources.list.d/bazel.list
    curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -
    sudo apt-get update && sudo apt-get install -y bazel
else
    echo 'Install requirements failed!'
    echo "Not supported unix-like system $(uname)!"
    exit 1
fi

echo 'Install requirements Ok!'
bazel version
