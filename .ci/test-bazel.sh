#! /usr/bin/env bash

# The test cases
readonly tests=(
    "tests:argc_test"
    "tests:clog_test"
    "tests:sdb_test"
    "tests:bits_test"
)

bazel coverage --collect_code_coverage "${tests[@]}"

# workaround to generate gcda files
readonly tests_=(
    "tests/argc_test"
    "tests/clog_test"
    "tests/sdb_test"
    "tests/bits_test"
)

for t in $tests_
do
    ./bazel-bin/$t
done
