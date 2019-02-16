project('lasso', 'cpp',
    version: '0.3.0',
    license: 'Unlicense',
    default_options: ['cpp_std=c++2a'])

install_headers('include/lasso.h', subdir: 'lasso/')

if get_option('example')
    subdir('example/')
endif