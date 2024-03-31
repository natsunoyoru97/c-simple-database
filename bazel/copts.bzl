# TODO(hjiang): Use crosstools.bzl.
CC_OPTIONS = [
  # Enforce compiler version.
  "--std=c++2a",
  # Warning options.
  "-Werror",
  "-Wall",
  "-Wextra",
  "-Wcast-qual",
  "-Wconversion-null",
  "-Wmissing-declarations",
  "-Woverlength-strings",
  "-Wpointer-arith",
  "-Wunused-local-typedefs",
  "-Wunused-result",
  "-Wvarargs",
  "-Wvla",
  "-Wwrite-strings",
  "-Wno-missing-field-initializers",
  "-Wno-sign-compare",
]
