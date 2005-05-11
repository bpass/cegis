###############################################################################
#
# Programmer: Mark Schisler
#             USGS, M.C.M.C.
#
# Project   : MOSIX Projection Project
#
# File      : Makefile
#
# Purpose   : Makefile for the main project.
#
# Date Last Modified: Thu Feb 24 09:58:47 CST 2005
#
###############################################################################

.PHONY: all clean

SHELL = /bin/sh

#set our prefix to the current directory
PREFIX = /work/mosixFastProjections

#this is the directory where the projector projector is
PRJRDIR = $(PREFIX)/include/Projector

INCPATHS = -I$(PREFIX)/include \
		   -I/usr/src/linux/include \
		   -I$(PREFIX)/include/Projector

CXX = g++
CXXFLAGS = -g -Wall -W -pthread -Wno-deprecated $(INCPATHS)

# -D_MOSIX will activate mosix-specific tidbits,like locking migration
#  for socket communication etc. if running in a mosix environment, it's
#  a really good idea

DEFS = #-D_MOSIX

LIBDIRS = -L$(PREFIX)/lib -L/usr/local/lib -L/usr/lib

OBJDIR = .obj

MLIBS = -lProjectionMesh -lMathLib -lProjectionIO -lImageLib \
        -lgeotiff -ltiff -lProjection -lMiscUtils \
        -lpthread -lgctpc -lboost_thread 
SLIBS = -lProjectionMesh -lMathLib -lProjectionIO -lImageLib \
        -lgeotiff -ltiff -lProjection  -lgctpc -lMiscUtils 

# gnu make specific
SOURCES = $(wildcard *.cpp) 
HEADERS = $(wildcard *.h)

POBJECTS = $(PRJRDIR)/$(OBJDIR)/ProjectionParams.o \
		   $(PRJRDIR)/$(OBJDIR)/Projector.o \
		   $(PRJRDIR)/$(OBJDIR)/ProjectorException.o \
		   $(PRJRDIR)/$(OBJDIR)/ProjUtil.o 

MOBJECTS = ${OBJDIR}/SlaveManager.o \
		   ${OBJDIR}/WorkManager.o \
		   ${OBJDIR}/WorkUnit.o \
		   ${OBJDIR}/Pulsar.o \
		   ${OBJDIR}/BigJob.o \
		   ${OBJDIR}/mastermain.o \
		   ${OBJDIR}/StitcherNode.o \
		   ${OBJDIR}/Stitcher.o \
		   ${OBJDIR}/inparms.o \
		   $(POBJECTS)

SOBJECTS = ${OBJDIR}/slavemain.o \
		   ${OBJDIR}/WorkUnit.o \
		   ${OBJDIR}/Pulsar.o \
		   ${OBJDIR}/inparms.o \
		   $(POBJECTS)

.SUFFIXES: .cpp 

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(DEFS) -o $@ $<

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

default: projector master slave

projector:  
	cd include/Projector; \
	${MAKE} "CXX=${CXX}" "CXXFLAGS=${CXXFLAGS}" "DEFS=${DEFS}" \
	" OBJDIR=${OBJDIR}" all \
	"INCPATHS=${INCPATHS}" 

master: projector $(MOBJECTS)
	$(CXX) $(CXXFLAGS) $(MOBJECTS) -o $@ $(LIBDIRS) $(MLIBS)

slave: projector $(SOBJECTS)
	$(CXX) $(CXXFLAGS) $(SOBJECTS) -o $@ $(LIBDIRS) $(SLIBS)

clean:
	-@rm -f core
	-@rm -f master
	-@rm -f slave
	-@rm -f .obj/*.o
	-@rm -f Makefile.dep
	-@cd include/Projector; make clean

# Automatically generate dependencies and include them in Makefile
depend: Makefile.dep
	
Makefile.dep: $(SOURCES)
	cat </dev/null >Makefile.dep ; \
	$(CXX) $(INCPATHS) -E -MM $^ 2>/dev/null | \
	perl -pe 's!^([^:]+:)!$(OBJDIR)/$$1!' > $@ 

-include Makefile.dep

