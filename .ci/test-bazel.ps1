#! powershell

# The test cases
$tests=("tests:argc_test", "tests:clog_test", "tests:sdb_test", "tests:bits_test")

~\scoop\apps\bazel\current\bazel test --test_output=errors $tests[0..$tests.Count]
