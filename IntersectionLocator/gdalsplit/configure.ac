dnl @file configure.ac
dnl @author Austin Hartman
dnl $Id: configure.ac,v 1.1 2005/12/02 19:28:46 ahartman Exp $

AC_INIT(gdalsplit, 0.1, ahartman@usgs.gov)
AC_PREREQ(2.54)
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

dnl
dnl Look for Frank Warmerdam's GDAL library
dnl
AC_CHECK_PROG( GDAL_CONFIG, gdal-config, yes,
               AC_MSG_ERROR([No GDAL library.  Get from http://remotesensing.org/gdal/]) )
if test x${GDAL_CONFIG} = "xyes"; then
  GDAL_CFLAGS=`gdal-config --cflags`
  GDAL_LIBS=`gdal-config --libs`
fi

AC_SUBST(GDAL_CFLAGS)
AC_SUBST(GDAL_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

