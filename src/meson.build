sources = [
  'main.c',
  'ojo-player.c',
  'ojo-playlist.c',
  'ojo-settings.c',
  'ojo-window.c',
  'ojo-track.c',
  'ojo-controlbox.c',
]

add_project_arguments('-rdynamic', '-Wl,--export-dynamic', language : 'c')

libgtk = dependency('gtk+-3.0')
libvlc = dependency('libvlc')
gmoddep = dependency('gmodule-2.0')

executable('ojo', sources, link_args : ['-lX11', '-lm'], dependencies : [libgtk, libvlc, gmoddep], install: true)
