project('light', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3', # -Wall -Wextra
    'werror=true',
    'cpp_std=c++20'
  ]
)

# Declare project
light_lib = static_library(
  'light',
  sources: [
    'include/erased.hpp',
    'include/light.hpp',
    'include/news.hpp',
    'src/light.cpp',
  ],
  include_directories: include_directories('include', 'src')
)

# Generate pkg-config file
pkgconfig = import('pkgconfig')
pkg = pkgconfig.generate(
  libraries: light_lib,
  version: meson.project_version(),
  description: 'the light project',
  name: meson.project_name(),
  filebase: meson.project_name(),
  subdirs: ['src', 'include']
)

# Dependency for Catch2
catch2_dep = dependency('catch2-with-main', required: true, method: 'pkg-config')

# Declare a tests project
test_exe = executable(
  'tests',
  sources: [
    'tests/erased.cpp',
    'tests/light.cpp',
  ],
  link_with: [light_lib],
  dependencies: [catch2_dep],
  include_directories: include_directories('include')
)
test('light', test_exe)
