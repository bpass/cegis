dnl @file configure.ac
dnl @author Austin Hartman
dnl $Id: configure.ac,v 1.3 2005/09/21 19:04:47 ahartman Exp $

# Do the initialization stuff
AC_INIT(CurveShoulderFinder, 0.1, ahartman@usgs.gov)
AC_PREREQ(2.54)
AM_INIT_AUTOMAKE

# Make sure we're using C++ as the language
AC_PROG_CXX
AC_LANG([C++])

# Check for boost::smart_ptr
AC_ARG_WITH([boost],
            AC_HELP_STRING([--with-boost=path],[path to Boost C++ library headers]),
            BOOST_PATH="$withval")

if ! test -z "$BOOST_PATH"; then
    CPPFLAGS="$CPPFLAGS -I${BOOST_PATH}"
fi

AC_CHECK_HEADERS([boost/smart_ptr.hpp],,
                 AC_MSG_ERROR([No Boost C++ library -- get from http://www.boost.org]))
             
# Check for GDAL
AC_CHECK_PROG(GDAL_CONFIG, gdal-config, yes,
              AC_MSG_ERROR([No "gdal-config" in your path.  Get from http://remotesensing.org/gdal/]))

if test x${GDAL_CONFIG} = "xyes"; then
  GDAL_CFLAGS=`gdal-config --cflags`
  GDAL_LIBS=`gdal-config --libs`
fi

# Place the definitions of the following macros in the Makefiles
AC_SUBST(CPPFLAGS)
AC_SUBST(GDAL_LIBS)
AC_SUBST(GDAL_CFLAGS)

# Output the Makefiles
AC_CONFIG_FILES([Makefile drivers/Makefile tests/Makefile])
AC_OUTPUT

