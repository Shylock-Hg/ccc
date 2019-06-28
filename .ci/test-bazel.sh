#! /usr/bin/env bash

set -ex;

# The test cases
readonly tests=(
    "tests:argc_test"
    "tests:clog_test"
    "tests:sdb_test"
    "tests:bits_test"
    "tests:ownership_test"
)

bazel test --test_output=errors "${tests[@]}"
