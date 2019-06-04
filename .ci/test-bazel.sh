#! /usr/bin/env bash

# The test cases
readonly tests=(
    "tests:argc_test"
    "tests:clog_test"
    "tests:sdb_test"
    "tests:bits_test"
)

bazel coverage --collect_code_coverage --test_output=errors "${tests[@]}"
