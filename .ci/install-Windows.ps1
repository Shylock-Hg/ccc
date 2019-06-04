#! powershell

iex (new-object net.webclient).downloadstring('https://get.scoop.sh')
scoop install bazel
bazel version
