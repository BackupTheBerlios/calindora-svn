# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Calindora], [0.0.1])
AM_INIT_AUTOMAKE(1.8)
AC_CONFIG_SRCDIR([src/Calindora.cpp])
AM_CONFIG_HEADER([config.h])

AM_OPTIONS_WXCONFIG

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

AM_PATH_WXCONFIG(2.5.2, WXFOUND=1)

if test "$WXFOUND" != 1; then
	AC_MSG_ERROR([
		Please check that wx-config is in the path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' command) is in LD_LIBRARY_PATH or
		equivalent variable and wxWidgets is version 2.5.2 or above.
	])
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY -Wall"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"

LDFLAGS="$LDFLAGS $WX_LIBS"

AC_SUBST(LDFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
