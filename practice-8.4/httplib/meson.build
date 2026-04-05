project('httplib', 'c',
    version: '1.0.0',
    default_options: ['c_std=c11'])

inc = include_directories('include')

sources = [
    'src/main.c',
    'src/http_request.c',
    'src/http_response.c'
]

executable('httplib',
    sources,
    include_directories: inc,
    install: true)

# Статическая библиотека
lib_sources = [
    'src/http_request.c',
    'src/http_response.c'
]

http_lib = static_library('http',
    lib_sources,
    include_directories: inc,
    install: true)

http_dep = declare_dependency(
    include_directories: inc,
    link_with: http_lib)

# Для тестов
test('httplib_test', executable('httplib_test', sources, include_directories: inc))