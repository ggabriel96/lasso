sfml_graphics = dependency('sfml-graphics')
sfml_window = dependency('sfml-window')
sfml_system = dependency('sfml-system')

nested_exe = executable('nested', 'test.cpp',
                        link_with: lassolib,
                        cpp_args: '-fconcepts',
                        dependencies: [sfml_graphics, sfml_window, sfml_system])
