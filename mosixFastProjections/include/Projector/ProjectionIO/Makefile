# Generated automatically from Makefile.in by configure.
SHELL			=	/bin/sh

prefix			=	/home/mschisler/MSX
exec_prefix		=	${prefix}
host_os			=	linux-gnu
srcdir			=	.
top_srcdir		=	.
enable_debug		=	no


# Where to find includes for libraries that ProjectionIO depends on.
INCPATHS = -I$(prefix)/include  -I${prefix}/include/Projector

# Where to install ProjectionIO include files.
INCDEST = $(prefix)/include/ProjectionIO

# Where to install ProjectionIO's library.
LIBDEST = $(prefix)/lib

ifeq ($(enable_debug),yes)
DEBUG= -g -Wall
else
DEBUG= -O2
endif

CC= gcc
CXX= c++
CXXFLAGS= $(NOUCHAR) $(DEBUG) $(INCPATHS)
RANLIB=ranlib

SRCS=	ProjectionIOException.cpp	\
	ProjectionReader.cpp            \
	ProjectionWriter.cpp        

OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: libProjectionIO.a
	@echo "libProjectionIO.a successfully built."

libProjectionIO.a: $(OBJS)
	ar rsu libProjectionIO.a $(OBJS)

install: libProjectionIO.a
	$(top_srcdir)/config/mkinstalldirs $(INCDEST)
	$(top_srcdir)/config/mkinstalldirs $(LIBDEST)
	cp *.h $(INCDEST)
	cp libProjectionIO.a $(LIBDEST)

clean::
	rm -f libProjectionIO.a core *~ $(OBJS)

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log

uninstall:
	rm -rf $(INCDEST)
	rm -f $(LIBDEST)/libProjectionIO.a

# Automatically rerun configure if the .in files have changed
$(srcdir)/configure:	configure.in
	cd $(srcdir) && autoconf

$(srcdir)/stamp-h:  config.status
	./config.status

$(srcdir)/Makefile: Makefile.in config.status
	./config.status

$(srcdir)/config.status: configure
	./config.status --recheck

