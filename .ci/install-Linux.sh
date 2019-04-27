#! /usr/bin/env sh

# Install bazel, valgrind in linux

set -eu

if [ "${TRAVIS_OS_NAME}" != linux ]; then
    echo "Not a Linux build; skipping installation"
    exit 0
fi


if [ "${TRAVIS_SUDO}" = "true" ]; then
    echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | \
        sudo tee /etc/apt/sources.list.d/bazel.list
    curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -
    sudo apt-get update && sudo apt-get install -y bazel
elif [ "${CXX}" = "clang++" ] || [ "${CC}" = "clang" ]; then
    # Use ccache, assuming $HOME/bin is in the path, which is true in the Travis build environment.
    ln -sf /usr/bin/ccache $HOME/bin/${CXX};
    ln -sf /usr/bin/ccache $HOME/bin/${CC};
fi
