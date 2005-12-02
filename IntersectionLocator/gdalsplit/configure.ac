dnl @file configure.ac
dnl @author Austin Hartman
dnl $Id: configure.ac,v 1.2 2005/12/02 20:17:33 ahartman Exp $

AC_INIT(gdalsplit, 0.1, hrz@geodata.soton.ac.uk)
AC_PREREQ(2.54)
AM_INIT_AUTOMAKE

AC_PROG_CXX

dnl
dnl Look for Frank Warmerdam's GDAL library
dnl

AC_CHECK_PROG( GDAL_CONFIG, gdal-config, yes,
               AC_MSG_ERROR([No GDAL library.  Get from http://remotesensing.org/gdal/]) )


if test x${GDAL_CONFIG} = "xyes"; then
  GDAL_CFLAGS=`gdal-config --cflags`
  GDAL_LIBS=`gdal-config --libs`
fi

dnl
dnl Finished looking for Frank Warmerdam's GDAL library
dnl

AC_SUBST(GDAL_CFLAGS)
AC_SUBST(GDAL_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

