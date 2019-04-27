#! /usr/bin/env bash

# The test cases
readonly tests=(
    "tests:argc_test"
    "tests:clog_test"
)

bazel test "${tests[@]}"
