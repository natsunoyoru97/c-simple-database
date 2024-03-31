workspace(name = "bipolar")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("//bazel:dependencies.bzl", "bipolar_dependencies")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    sha256 = "74d544d96f4a5bb630d465ca8bbcfe231e3594e5aae57e1edbf17a6eb3ca2506",
    urls = [
        "https://storage.googleapis.com/mirror.tensorflow.org/github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

bipolar_dependencies()

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "05f4598a4e5589fbbd0929f7a2bb96efe51cbb37",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1575786499 -0800",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()
