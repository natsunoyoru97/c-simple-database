load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
    "new_git_repository",
)
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//bazel/glog:workspace.bzl", "com_glog_workspace")
load("//bazel/abseil:workspace.bzl", "com_github_abseil")

def bipolar_dependencies():
    _com_github_google_googletest()
    _com_github_google_benchmark()
    _com_github_axboe_liburing()
    _com_github_fmtlib_fmt()
    _com_github_gabime_spdlog()
    com_github_abseil()
    com_glog_workspace()
    _com_gflag_workspace()
    _com_google_protobuf()
    _com_rules_python()
    _zlib_workspace()

def _com_github_google_googletest():
    http_archive(
        name = "gtest",
        sha256 = "da0f704133e746bb248358ce05322ec9013c3c32449408e9235b4ebf23dbf6b6",
        strip_prefix = "googletest-ca4b7c9ff4d8a5c37ac51795b03ffe934958aeff",
        urls = ["https://github.com/google/googletest/archive/ca4b7c9ff4d8a5c37ac51795b03ffe934958aeff.tar.gz"],
    )

def _com_github_google_benchmark():
    git_repository(
        name = "benchmark",
        remote = "https://github.com/google/benchmark",
        tag = "v1.5.0",
    )

def _com_github_axboe_liburing():
    """
    linux/io_uring.h shipped by distribution is old
    """
    new_git_repository(
        name = "liburing",
        remote = "https://github.com/axboe/liburing",
        commit = "556960942eaa69fd53544932f00db3fa9f196e00",
        build_file = "@bipolar//bazel/external:liburing.BUILD",
    )

def _com_github_fmtlib_fmt():
    new_git_repository(
        name = "fmtlib",
        remote = "https://github.com/fmtlib/fmt",
        tag = "6.1.1",
        build_file = "@bipolar//bazel/external:fmtlib.BUILD",
    )

def _com_github_gabime_spdlog():
    new_git_repository(
        name = "spdlog",
        remote = "https://github.com/gabime/spdlog",
        tag = "v1.4.2",
        build_file = "@bipolar//bazel/external:spdlog.BUILD",
    )

def _com_gflag_workspace():
    http_archive(
        name = "com_github_gflags_gflags",
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        strip_prefix = "gflags-2.2.2/",
        urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
    )

def _com_google_protobuf():
    http_archive(
        name = "com_google_protobuf",
        sha256 = "4cc96077065ad39000fc75529e0b8a6b3a9458c5aef658d8e78bc21f6d6173bc",
        strip_prefix = "protobuf-3.19.5",
        urls = ["https://github.com/protocolbuffers/protobuf/releases/download/v3.19.5/protobuf-cpp-3.19.5.tar.gz"],
    )

def _com_rules_python():
    rules_python_version = "740825b7f74930c62f44af95c9a4c1bd428d2c53" # Latest @ 2021-06-23

    http_archive(
        name = "rules_python",
        sha256 = "09a3c4791c61b62c2cbc5b2cbea4ccc32487b38c7a2cc8f87a794d7a659cc742",
        strip_prefix = "rules_python-{}".format(rules_python_version),
        url = "https://github.com/bazelbuild/rules_python/archive/{}.zip".format(rules_python_version),
    )

def _zlib_workspace():
    http_archive(
        name = "zlib",
        build_file = "//bazel:zlib.BUILD",
        sha256 = "b3a24de97a8fdbc835b9833169501030b8977031bcb54b3b3ac13740f846ab30",
        strip_prefix = "zlib-1.2.13",
        urls = [
            "https://github.com/madler/zlib/releases/download/v1.2.13/zlib-1.2.13.tar.gz",
        ],
    )
