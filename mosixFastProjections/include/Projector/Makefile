###############################################################################
#
# Programmer: Mark Schisler
#             USGS, M.C.M.C.
#
# Project   : MOSIX Projection Project
#
# File      : Makefile 
#
# Purpose   : Makefile for the Projector portion of the project
#
# Date Last Modified: Thu Feb 24 09:58:47 CST 2005
#
###############################################################################

# this Makefile is recursively called.  Not all MACROS defined here

OBJDIR = .obj
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp= .obj/%.o)

.SUFFIXES: .cpp .o

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) ${INCPATHS} $(DEFS) -c -o $@ $<

default: all

all: $(OBJECTS) 

depend: Makefile.dep
	
Makefile.dep: $(SOURCES)
	cat </dev/null >Makefile.dep ; \
	$(CXX) $(INCPATHS) -E -MM $^ 2>/dev/null | \
	perl -pe 's!^([^:]+:)!$(OBJDIR)/$$1!' > $@ 

clean : 
	-@rm -rf .obj/*.o

-include Makefile.dep

