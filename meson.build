project('my-projects', ['cpp'], default_options : ['cpp_std=c++14', 'c_std=c99'])

qt5_mod = import('qt5')
qt5_core_dep = dependency('qt5', modules: ['Core'])
qt5_gui_dep = dependency('qt5', modules: ['Gui'])
qt5_widgets_dep = dependency('qt5', modules: ['Widgets'])

subdir('src')
