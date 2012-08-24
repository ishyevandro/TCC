AC_INIT(TCC Evandro,1.0.0,email@email,tcc-evandro)
AC_PREREQ([2.50])
AM_INIT_AUTOMAKE([dist-lzma gnu 1.11 -Wall])
AC_PROG_CXX
#AC_LANG ([C++])
AC_HEADER_STDC
AC_CHECK_HEADER([iostream vector ])
PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4 >= 2.10])
AX_BOOST_REGEX
AC_TYPE_SIZE_T
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
