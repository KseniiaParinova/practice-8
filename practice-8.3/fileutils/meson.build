project('fileutils', 'c',
    version: '1.0.0',
    default_options: ['c_std=c11'])

inc = include_directories('include')

sources = [
    'src/main.c',
    'src/path.c',
    'src/io.c'
]

executable('fileutils',
    sources,
    include_directories: inc,
    install: true)

# Для использования как библиотеки
lib_sources = [
    'src/path.c',
    'src/io.c'
]

fileutils_lib = static_library('fileutils',
    lib_sources,
    include_directories: inc,
    install: true)

fileutils_dep = declare_dependency(
    include_directories: inc,
    link_with: fileutils_lib)