prefix		=	/home/mschisler/MSX/FrodoLib/..
exec_prefix	=	${prefix}
host_os		=	linux
srcdir		=	.
top_srcdir	=	.
gen_depend	=	no
enable_debug	=	no
enable_purify	=	no

foundone := false
# DG/UX (Data General UNIX)
ifeq ($(findstring dgux,$(host_os)),dgux)
INCPATHS = -I$(prefix)/include
LIBPATHS = -L$(prefix)/lib
LIBS = $(LIBPATHS) -ldrg -lDRGSourceInfo -lm
ORALIBS = -lsql -lsqlnet -lora -lnetwork -lcore -lm
foundone := true
endif

# Linux
ifeq ($(host_os),linux)
INCPATHS = -I/usr/X11R6/include -I$(prefix)/include
LIBPATHS = -L/usr/X11R6/lib -L$(prefix)/lib
LIBS = $(LIBPATHS) -lm
ORALIBS = 
foundone := true
endif

ifeq ($(foundone),false)
INCPATHS = -I$(prefix)/include
LIBPATHS = -L$(prefix)/lib
LIBS = $(LIBPATHS) -lm
ORALIBS = 
endif

# Where to find includes for libraries that FrodoLib depends on.
INCPATHS = -I$(prefix)/include

# Where to install drg's include files.
INCDEST = $(prefix)/include/FrodoLib

# Where to install drg's library.
LIBDEST = $(prefix)/lib

ifeq ($(enable_debug),yes)
DEBUG = -g
else
DEBUG = -O2
endif

ifeq ($(enable_purify),yes)
PURIFY = purify
else
PURIFY =
endif

CC=$(PURIFY) gcc
CXX=$(PURIFY) g++
CXXFLAGS= $(DEBUG) $(INCPATHS)

INSTALLFILE = $(srcdir)/config/install-sh -c
INSTALLDIR  = $(srcdir)/config/install-sh -d

SRCS=   SpinnerThingy.cc 

OBJS=$(SRCS:.cc=.o)

all: installinc libFrodo.a installlib 
	@@echo "Done."

libFrodo.a: $(OBJS)
	ar ru libFrodo.a $(OBJS)

# Preserve the time stamp on the include files.
installinc: 
	@@echo "Installing includes"
	@@test -d $(INCDEST) || $(INSTALLDIR) $(INCDEST)
	@@tar cf /tmp/drg.tar *.h ; \
	cd $(INCDEST); tar xf /tmp/drg.tar ; \
	rm -rf /tmp/drg.tar

installlib:
	@@echo "Installing library"
	@@test -d $(LIBDEST) || $(INSTALLDIR) $(LIBDEST)
	@@cp libFrodo.a $(LIBDEST)

clean::
	rm -f $(OBJS) *.a core 

realclean: 
	rm -rf *.d config.cache config.status config.log


ifeq ($(gen_depend),yes)
# Automatic generation of dependencies
%.d: %.cc
	@@echo "Regenerating dependencies for $<";$(SHELL) -ec '$(CC) \
	$(INCPATHS) -MM $(CPPFLAGS) $< | sed '\''s/$*.o/& $@@/g'\'' > $@@'


include $(SRCS:.cc=.d)

endif
