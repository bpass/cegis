# Generated automatically from Makefile.in by configure.
# $Id: Makefile,v 1.1 2005/03/11 23:59:10 mschisler Exp $
# Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $
SHELL		= /bin/sh

prefix		= /home/mschisler/MSX
exec_prefix	= ${prefix}
host_os		= linux-gnu
srcdir		= .
top_srcdir	= .
enable_debug	= no

# Set up the include paths
INCPATHS = -I$(prefix)/include -I${prefix}/include/Projector

# Where to install the include files
INCDEST = $(prefix)/include/ProjectionMesh

# Where to install the library files
LIBDEST = $(prefix)/lib

# Set our compiler options
ifeq ($(enable_debug),yes)
DEBUG = -g -Wall 
else
DEBUG = -O2
endif

# Compiler and other defs
CC		= gcc
CXX		= c++
CXXFLAGS	= $(DEBUG) $(INCPATHS) -Wno-deprecated
RANLIB		= ranlib

SRCS =	PmeshException.cpp	\
	ProjectionMesh.cpp	\
	MeshNode.cpp

# Dependencies for the program
OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix Rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: libProjectionMesh.a
	@@echo "libProjectionmesh.a successfully built."

libProjectionMesh.a: $(OBJS)
	ar rsu libProjectionMesh.a $(OBJS)

install: libProjectionMesh.a
	$(top_srcdir)/config/mkinstalldirs $(INCDEST)
	$(top_srcdir)/config/mkinstalldirs $(LIBDEST)
	cp *.h $(INCDEST)
	cp libProjectionMesh.a $(LIBDEST)

clean::
	rm -f libProjectionMesh.a core *~ $(OBJS)

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log

uninstall:
	rm -rf $(INCDEST)
	rm -f $(LIBDEST)/libProjectionMesh.a

# Automatically rerun configure if the .in files have changed
$(srcdir)/configure:	configure.in
	cd $(srcdir) && autoconf

$(srcdir)/stamp-h:  config.status
	./config.status

$(srcdir)/Makefile: Makefile.in config.status
	./config.status

$(srcdir)/config.status: configure
	./config.status --recheck












