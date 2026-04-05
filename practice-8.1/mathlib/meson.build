project('mathlib', 'c',
    version: '1.0.0',
    default_options: [
        'warning_level=3',
        'c_std=c11'
    ]
)

inc = include_directories('include')

cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required: false)

lib_src = files(
    'src/basic.c',
    'src/vector.c',
    'src/matrix.c'
)

mathlib = static_library('mathlib', lib_src,
    include_directories: inc,
    dependencies: [math_dep]
)

executable('mathlib_demo', 'src/main.c',
    include_directories: inc,
    link_with: mathlib,
    dependencies: [math_dep]
)

test_exe = executable('test_mathlib', 'tests/test_mathlib.c',
    include_directories: inc,
    link_with: mathlib,
    dependencies: [math_dep]
)

test('mathlib tests', test_exe)