DB_LLVM_ASAN_FLAGS = {
    "-fsanitize=address",
}

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

set_languages("cxx17")
add_requires("doctest", "gtest", "glog", "gflags")
add_rules("mode.debug", "mode.release")

-- Pre-compiled Abseil is NOT PREFERRED, see https://github.com/abseil/abseil-cpp/blob/master/FAQ.md#what-is-abi-and-why-dont-you-recommend-using-a-pre-compiled-version-of-abseil.
-- Abseil should be built with the flag "-std=c++17", see https://github.com/abseil/abseil-cpp/issues/819.
-- TODO: Any better solution?
package("abseil")
    add_urls("https://github.com/abseil/abseil-cpp/archive/$(version).tar.gz",
             "https://github.com/abseil/abseil-cpp.git")
    add_versions("20220623.0", "4208129b49006089ba1d6710845a45e31c59b0ab6bff9e5788a87f55c5abd602")

    add_deps("cmake")

    add_links(
    "absl_status", "absl_cord", 
    "absl_flags", "absl_flags_parse", "absl_flags_internal", "absl_flags_reflection", "absl_flags_marshalling", 
    "absl_flags_commandlineflag_internal", "absl_synchronization", "absl_time", "absl_hash", "absl_city", "absl_time_zone", 
    "absl_spinlock_wait", "absl_failure_signal_handler", "absl_bad_optional_access", "absl_flags_commandlineflag", 
    "absl_random_internal_pool_urbg", 
    "absl_cordz_info", "absl_cord_internal", "absl_cordz_functions", "absl_cordz_handle", "absl_cordz_sample_token",
    "absl_base", "absl_bad_any_cast_impl", "absl_periodic_sampler", "absl_random_distributions",
    "absl_flags_usage_internal", "absl_random_seed_sequences", 
    "absl_throw_delegate", "absl_stacktrace", "absl_symbolize", "absl_debugging_internal", 
    "absl_flags_private_handle_accessor",
    "absl_strings", "absl_flags_config", "absl_malloc_internal", "absl_str_format_internal",
    "absl_flags_usage", "absl_strings_internal", "absl_flags_program_name", "absl_int128",
    "absl_scoped_set_env", "absl_raw_hash_set", "absl_random_internal_seed_material",
    "absl_random_internal_randen", "absl_random_internal_randen_slow", "absl_random_internal_randen_hwaes_impl", 
    "absl_random_internal_randen_hwaes",
    "absl_graphcycles_internal", "absl_exponential_biased", "absl_bad_variant_access", "absl_statusor", 
    "absl_random_internal_distribution_test_util", "absl_random_internal_platform", 
    "absl_hashtablez_sampler", "absl_demangle_internal", "absl_leak_check", "absl_log_severity", "absl_raw_logging_internal",
    "absl_strerror", "absl_examine_stack", "absl_low_level_hash", "absl_random_seed_gen_exception", "absl_civil_time")

    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DABSL_BUILD_TESTING=" .. ("ON"))
        table.insert(configs, "-DABSL_USE_GOOGLETEST_HEAD=" .. ("ON"))
        table.insert(configs, "-DCMAKE_CXX_STANDARD=" .. ("17"))
        import("package.tools.cmake").install(package, configs)
    end)
    on_test(function (package)
        os.run("ctest")
    end)
package_end()

add_requires("abseil")

option("asan")
    set_default(false)
    set_showmenu(true)
    add_ldflags(DB_LLVM_ASAN_FLAGS)

target("target")
    set_toolchains("clang")
    set_kind("binary")
    add_files("src/main.cpp")
    add_packages("abseil", "glog", "gflags")
    set_warnings("all", "error")
    add_deps("storage", "pager")
    add_cxxflags(DB_LLVM_TEST_FLAGS, DB_LLVM_FLAGS)
    add_options("asan")

target("pager")
    set_kind("static")
    add_packages("abseil")
    add_files("src/pager/pager.cpp")

target("storage")
    set_kind("static")
    add_packages("abseil")
    add_deps("pager")
    add_files("src/storage/storage.cpp")

target("test")
    set_toolchains("clang")
    set_kind("binary")
    add_files("test/main.cc")
    add_packages("abseil", "gtest")
    set_warnings("all", "error")
    add_cxxflags(DB_LLVM_TEST_FLAGS, DB_LLVM_EXCEPTIONS_FLAGS)
    add_deps("storage", "pager")
    add_options("asan")

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

