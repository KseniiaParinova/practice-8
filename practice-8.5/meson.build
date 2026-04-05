project('minitest', 'c',
    version: '1.0.0',
    default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files('src/minitest.c')

minitest = static_library('minitest', lib_src,
    include_directories: inc
)

executable('minitest_demo', 'src/main.c',
    include_directories: inc,
    link_with: minitest,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test_exe = executable('test_example', 'tests/test_example.c',
    include_directories: inc,
    link_with: minitest,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test('minitest example', test_exe)