includes = include_directories('.')

headers = [
    'MainWindow.h'
]

src = [
    'main.cpp',
    'MainWindow.cpp'
]

uis = [
    'MainWindow.ui'
]

processed = qt5_mod.preprocess(
    moc_headers : headers,
    ui_files : uis,
    include_directories : includes,
    qresources : 'notepad-xx.qrc',
)

notepad_exe = executable('notepad-xx',
  sources : src + processed,
  include_directories : includes,
  dependencies : [qt5_core_dep, qt5_gui_dep, qt5_widgets_dep,  qt5_printsupport_dep],
  gui_app : true,
)