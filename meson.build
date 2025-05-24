project('Ojo', 'c',
  version: '0.1',
  meson_version: '>= 0.57.0',
)

i18n = import('i18n')
gnome = import('gnome')
cc = meson.get_compiler('c')

glib_compile_schemas = find_program('glib-compile-schemas')

subdir('src')
subdir('data')

#meson.add_install_script('meson-post-install.py')

gnome.post_install(
  gtk_update_icon_cache: true,
  glib_compile_schemas: true,
  #update_desktop_database: true,
)
