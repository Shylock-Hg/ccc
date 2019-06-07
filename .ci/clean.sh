#! /usr/bin/env bash

set -ex;

# Bazel
bazel clean

# Make
sudo make uninstall
make clean
