AC_INIT(terranova, 0.90)

AC_CONFIG_HEADER([config.h])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

AM_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_CPP
AC_PROG_INSTALL

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.12
		  ,
		  ,AC_MSG_ERROR([GTK+-2.12 is required to compile TerraNova]))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

AC_ARG_ENABLE(paranoia,
	      [  --enable-paranoia       compile TerraNova with -Wall -Wno-error -ansi -pedantic],
	      [paranoia=$enableval],
	      [paranoia="yes"])

if test $paranoia = "yes"; then
	PARANOIA_CFLAGS="-Wall -Wno-error -ansi -pedantic"
	AC_SUBST(PARANOIA_CFLAGS)
fi

AC_ARG_ENABLE(deprecated,
	      [  --enable-deprecated     make use of deprecated GTK+2 Symbols],
	      [deprecated=$enableval],
	      [deprecated="no"])

if test $deprecated = "no"; then
	DEPRECATE_CFLAGS="-DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"
	AC_SUBST(DEPRECATE_CFLAGS)
fi

AC_CONFIG_FILES([
Makefile
terranova.pc
])

AC_OUTPUT

echo ""
echo "TerraNova v$VERSION"
echo "Paranoia CFLAGS: $paranoia"
echo "Deprecated Symbols: $deprecated"
echo "Prefix: $prefix"
echo ""
