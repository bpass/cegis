# Generated automatically from Makefile.in by configure.
# Generated automatically from Makefile.in by configure.
SHELL		=	/bin/sh

prefix		=	/home/mschisler
exec_prefix	=	${prefix}
host_os		=	linux-gnu
srcdir		=	.
top_srcdir	=	.
enable_debug	=	no

# Where to find includes for libraries that MathLib depends on.
INCPATHS = -I$(prefix)/include

# Where to install MathLib's include files.
INCDEST = $(prefix)/include/MathLib

# Where to install MathLib's library.
LIBDEST = $(prefix)/lib

ifeq ($(enable_debug),yes)
DEBUG= -g -Wall
else
DEBUG= -O2
endif

CC= gcc
CXX= c++
CXXFLAGS= $(DEBUG) $(INCPATHS)
RANLIB=ranlib

SRCS=	RandomParkMiller.cpp			\
	RandomParkMillerBD.cpp			\
	RandomLEcuyerBD.cpp			\
	RandomKnuth.cpp				\
	Interpolator.cpp			\
	PolynomialInterpolator.cpp		\
	RationalInterpolator.cpp		\
	MathException.cpp			\
	CubicSplineInterpolator.cpp		\
	LinearLeastSquaresInterpolator.cpp 	\
	BiPolynomialInterpolator.cpp       	\
	LeastSquaresPlaneInterpolator.cpp  	\
	DlgViewerInterpolator.cpp               \
	BiLinearInterpolator.cpp                \
	BiCubicInterpolator.cpp                 \
        BiCubicSplineInterpolator.cpp


OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: libMathLib.a
	@@echo "libMathLib.a successfully built."

libMathLib.a: $(OBJS)
	ar rsu libMathLib.a $(OBJS)

install: libMathLib.a
	$(top_srcdir)/config/mkinstalldirs $(INCDEST)
	$(top_srcdir)/config/mkinstalldirs $(LIBDEST)
	cp *.h $(INCDEST)
	cp libMathLib.a $(LIBDEST)

clean::
	rm -f libMathLib.a core *~ $(OBJS)

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log

uninstall:
	rm -rf $(INCDEST)
	rm -f $(LIBDEST)/libMathLib.a

# Automatically rerun configure if the .in files have changed
$(srcdir)/configure:	configure.in
	cd $(srcdir) && autoconf

$(srcdir)/stamp-h:  config.status
	./config.status

$(srcdir)/Makefile: Makefile.in config.status
	./config.status

$(srcdir)/config.status: configure
	./config.status --recheck


