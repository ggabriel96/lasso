project('lasso', 'cpp',
    version: '0.3.0',
    license: 'Unlicense',
    default_options: ['cpp_std=c++2a'])

lassolib = static_library('lasso', 'include/lasso.h', install: true)

install_headers('include/lasso.h', subdir: 'lasso/')

if get_option('example')
    subdir('example')
endif