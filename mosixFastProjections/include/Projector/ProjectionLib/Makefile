# Generated automatically from Makefile.in by configure.
# $Id: Makefile,v 1.1 2005/03/11 23:59:10 mschisler Exp $
# Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

SHELL		=	/bin/sh

prefix		=	/home/mschisler/MSX
exec_prefix	=	${prefix}
host_os		=	linux-gnu
srcdir		=	.
top_srcdir	=	.
enable_debug	=	yes

# Where to find includes for libraries that ProjectionLib depends on.
INCPATHS = -I$(prefix)/include  -I${prefix}/include/Projector

# Where to install ProjectionLib's include files.
INCDEST = $(prefix)/include/ProjectionLib

# Where to install ProjectionLib's library.
LIBDEST = $(prefix)/lib

ifeq ($(enable_debug),yes)
DEBUG= -g -Wall
else
DEBUG= -O3 -fexpensive-optimizations -fomit-frame-pointer
endif

CC= gcc
CXX= c++
CXXFLAGS= $(DEBUG) $(INCPATHS) -Wno-deprecated

INSTALLFILE = $(top_srcdir)/config/install-sh -c
INSTALLDIR  = $(top_srcdir)/config/install-sh -d

SRCS= 	AlaskaConformalProjection.cpp    	\
	AlbersConicProjection.cpp		\
	AzimuthalEquidistantProjection.cpp	\
	AzimuthalProjection.cpp			\
	ConicProjection.cpp			\
	DatumConvertor.cpp			\
	EquidistantConicProjection.cpp		\
	EquirectangularProjection.cpp		\
	GeographicProjection.cpp		\
	GnomonicProjection.cpp			\
	HotineObliqueMercatorProjection.cpp	\
	LambertAzimuthalProjection.cpp		\
	LambertConformalConicProjection.cpp	\
	MercatorProjection.cpp			\
	MillerCylindricalProjection.cpp		\
	OrthographicProjection.cpp		\
	PolarStereographicProjection.cpp	\
	PolyconicProjection.cpp			\
	Projection.cpp				\
	ProjectionException.cpp			\
	PseudocylindricalProjection.cpp		\
	RobinsonProjection.cpp			\
	SinusoidalProjection.cpp		\
	StatePlaneProjection.cpp		\
	StereographicProjection.cpp		\
	TransverseMercatorProjection.cpp	\
	UTMProjection.cpp			\
	UnknownProjection.cpp			\
	VanDerGrintenProjection.cpp		\
	ZonedProjection.cpp                     \
	GoodeHomolosineProjection.cpp			



OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

#Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: libProjection.a
	@echo "libProjection.a successfully built"

libProjection.a: $(OBJS)
	ar rsu libProjection.a $(OBJS)

install: libProjection.a
	$(top_srcdir)/config/mkinstalldirs $(INCDEST)
	$(top_srcdir)/config/mkinstalldirs $(LIBDEST)
	cp *.h $(INCDEST)
	cp libProjection.a $(LIBDEST)

clean::
	rm -f $(OBJS) *.a core

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log

uninstall:
	rm -rf $(INCDEST)
	rm -f $(LIBDEST)/libProjection.a

# Automatically rerun configure if the .in files have changed
$(srcdir)/configure:	configure.in
	cd $(srcdir) && autoconf

$(srcdir)/Makefile: Makefile.in config.status
	./config.status

$(srcdir)/config.status: configure
	./config.status --recheck


