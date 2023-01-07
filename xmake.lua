DB_LLVM_EXCEPTIONS_FLAGS = {
    "-fexceptions",
}

DB_LLVM_FLAGS = {
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Weverything",
    "-Wno-c++98-compat-pedantic",
    "-Wno-conversion",
    "-Wno-covered-switch-default",
    "-Wno-deprecated",
    "-Wno-disabled-macro-expansion",
    "-Wno-double-promotion",
    "-Wno-comma",
    "-Wno-extra-semi",
    "-Wno-extra-semi-stmt",
    "-Wno-packed",
    "-Wno-padded",
    "-Wno-sign-compare",
    "-Wno-float-conversion",
    "-Wno-float-equal",
    "-Wno-format-nonliteral",
    "-Wno-gcc-compat",
    "-Wno-global-constructors",
    "-Wno-exit-time-destructors",
    "-Wno-nested-anon-types",
    "-Wno-non-modular-include-in-module",
    "-Wno-old-style-cast",
    "-Wno-range-loop-analysis",
    "-Wno-reserved-id-macro",
    "-Wno-shorten-64-to-32",
    "-Wno-switch-enum",
    "-Wno-thread-safety-negative",
    "-Wno-unknown-warning-option",
    "-Wno-unreachable-code",
    "-Wno-unused-macros",
    "-Wno-weak-vtables",
    "-Wno-zero-as-null-pointer-constant",
    "-Wbitfield-enum-conversion",
    "-Wbool-conversion",
    "-Wconstant-conversion",
    "-Wenum-conversion",
    "-Wint-conversion",
    "-Wliteral-conversion",
    "-Wnon-literal-null-conversion",
    "-Wnull-conversion",
    "-Wobjc-literal-conversion",
    "-Wno-sign-conversion",
    "-Wstring-conversion",
    "-Wthread-safety",
}

DB_LLVM_TEST_FLAGS = {
    "-std=c++17",
    "-Wno-c99-extensions",
    "-Wno-deprecated-declarations",
    "-Wno-missing-noreturn",
    "-Wno-missing-prototypes",
    "-Wno-missing-variable-declarations",
    "-Wno-null-conversion",
    "-Wno-shadow",
    "-Wno-shift-sign-overflow",
    "-Wno-sign-compare",
    "-Wno-unused-function",
    "-Wno-unused-member-function",
    "-Wno-unused-parameter",
    "-Wno-unused-private-field",
    "-Wno-unused-template",
    "-Wno-used-but-marked-unused",
    "-Wno-zero-as-null-pointer-constant",
    "-Wno-gnu-zero-variadic-macro-arguments",
}

set_languages("c++17")
set_toolset("cxx", "clang++")

add_requires("abseil", "doctest", "gtest ~1.12.1", "glog", "gflags")
add_rules("mode.debug", "mode.release")

target("target")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("abseil", "glog", "gflags")
    set_warnings("all", "error")
    add_cxxflags(DB_LLVM_FLAGS, DB_LLVM_EXCEPTIONS_FLAGS)

target("pager")
    set_kind("static")
    add_files("src/pager/pager.cpp")

target("storage")
    set_kind("static")
    add_files("src/storage/storage.cpp")

target("test")
    set_kind("binary")
    add_files("test/*.cc")
    add_packages("gtest")
    set_warnings("all", "error")
    add_cxxflags(DB_LLVM_TEST_FLAGS, DB_LLVM_EXCEPTIONS_FLAGS)

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

