sfml_graphics = dependency('sfml-graphics')
sfml_window = dependency('sfml-window')
sfml_system = dependency('sfml-system')

subdir('data/')

nested_exe = executable('nested', 'nested.cpp',
                        cpp_args: '-fconcepts',
                        dependencies: [sfml_graphics, sfml_window, sfml_system])
