# Generated automatically from Makefile.in by configure.
SHELL		=	/bin/sh

top_srcdir	=	.
srcdir		=	.
prefix		=	/work/usr
exec_prefix	=	${prefix}
bindir		=	$(exec_prefix)/bin
infodir		=	$(prefix)/info
libdir		=	$(prefix)/lib
mandir		=	$(prefix)/man
host_os		=	linux-gnu

enable_debug	=	no

# Where to find includes for libraries that MiscUtils depends on.
INCPATHS = -I$(prefix)/include

# Where to install MiscUtils's include files.
INCDEST = $(prefix)/include/MiscUtils

# Where to install MiscUtils's library.
LIBDEST = $(libdir)

ifeq ($(enable_debug),yes)
DEBUG= -g -Wall

else
DEBUG= -O2
endif

CXX= c++
CXXFLAGS= $(DEBUG) $(INCPATHS)
RANLIB=ranlib

SRCS=	cmp_nocase.cpp		\
	compare_nocase.cpp      \
	SpinnerThingy.cpp       \
	MicroTimer.cpp

OBJS=$(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp

# Suffix rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all: libMiscUtils.a
	@echo "libMiscUtils.a successfully built."

libMiscUtils.a: $(OBJS)
	ar rsu libMiscUtils.a $(OBJS)

install: libMiscUtils.a
	$(top_srcdir)/config/mkinstalldirs $(INCDEST)
	$(top_srcdir)/config/mkinstalldirs $(LIBDEST)
	cp *.h $(INCDEST)
	cp libMiscUtils.a $(LIBDEST)

clean::
	rm -f *.o *.a core *~ $(OBJS)

distclean: clean
	rm -f Makefile config.h config.status config.cache config.log

uninstall:
	rm -rf $(INCDEST)
	rm -f $(LIBDEST)/libMiscUtils.a


# Automatically rerun configure if the .in files have changed
$(srcdir)/configure:    configure.in
	cd $(srcdir) && autoconf

$(srcdir)/stamp-h: config.status
	./config.status

$(srcdir)/Makefile: Makefile.in config.status
	./config.status

$(srcdir)/config.status: configure
	./config.status --recheck   
