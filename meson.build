project('lasso', 'cpp',
    version: '0.4.0',
    license: 'Unlicense',
    default_options: ['cpp_std=c++20'])

install_headers('include/lasso.h', subdir: 'lasso/')

if get_option('examples')
    subdir('examples/')
endif
