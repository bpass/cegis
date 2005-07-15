## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.4 2005/07/15 20:55:33 ahartman Exp $
#

noinst_PROGRAMS = IntersectionLocator
IntersectionLocator_SOURCES = IntersectionLocator.cpp
IntersectionLocator_LDADD = -L. -lIntersectionLocator $(GDAL_LIBS)
IntersectionLocator_LDFLAGS = -no-undefined \
                              -static \
                              --enable-runtime-pseudo-reloc

noinst_LTLIBRARIES = libIntersectionLocator.la
libIntersectionLocator_la_SOURCES = Circle.cpp \
                                    Classifier.cpp \
                                    DelauneyTriangulator.cpp \
                                    DistanceFilter.cpp \
                                    InMemRaster.cpp \
                                    IntersectionMap.cpp \
                                    QuarticTriangulator.cpp \
                                    RecursiveTriangulator.cpp \
                                    SaalfeldRubberSheet.cpp \
                                    Template.cpp \
                                    VMFilter.cpp
libIntersectionLocator_la_LIBADD = $(GDAL_LIBS) -L/usr/local/bin -lproj
libIntersectionLocator_la_LDFLAGS = -no-undefined \
                                    -static \
                                    --enable-runtime-pseudo-reloc

INCLUDES = $(GDAL_CFLAGS)

# This is placed here because it's required by the program in the 
# TrainClassifier subdirectory, and this tells automake to try to
# build it first before going there
BUILT_SOURCES = libIntersectionLocator.la

#SUBDIRS = TrainClassifier
