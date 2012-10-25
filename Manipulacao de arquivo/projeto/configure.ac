AC_INIT(TCC-evandro, 1.0.1, 017706@comp.uems.br , tcc-evandro, http://moodle.alcionesytes.net/)
AC_PREREQ([2.50])
AM_INIT_AUTOMAKE([dist-lzma gnu 1.11 -Wall])
AC_PROG_CXX
#AC_LANG([C++])
AC_HEADER_STDC
AC_CHECK_HEADERS([iostream string fstream vector])
PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4 >= 2.10])
PKG_CHECK_MODULES([SQLITE], [sqlite3 >= 3.0])
PKG_CHECK_MODULES([EX], [libex-1.0 >= 1.0])
PKG_CHECK_MODULES([BSL], [libbancosl-1.0 >= 1.0])
AC_DEFINE([DB_FILE],["consulta.db"],[Arquivo da Base de Dados])
AX_BOOST_REGEX
AC_TYPE_SIZE_T
AC_PREFIX_DEFAULT([~/bin])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
