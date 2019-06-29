#! /usr/bin/env bash

set -ex;

bazel test --test_output=errors //tests/...
