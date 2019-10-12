AC_INIT([ojo], [0.2])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

GTK_REQUIRED=3.24.0

PKG_CHECK_MODULES(GTK, gtk+-3.0 >= $GTK_REQUIRED)

AC_CHECK_LIB(vlc, libvlc_new)
AC_CHECK_LIB(X11, XInitThreads)

AC_OUTPUT
