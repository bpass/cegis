## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.1 2005/07/14 00:29:32 ahartman Exp $
#

noinst_PROGRAMS = IntersectionLocator
IntersectionLocator_SOURCES = Circle.cpp \
                              Classifier.cpp \
                              DelauneyTriangulator.cpp \
                              DistanceFilter.cpp \
                              InMemRaster.cpp \
                              IntersectionLocator.cpp \
                              IntersectionMap.cpp \
                              QuarticTriangulator.cpp \
                              RecursiveTriangulator.cpp \
                              SaalfeldRubberSheet.cpp \
                              Template.cpp \
                              VMFilter.cpp
IntersectionLocator_LDADD = $(GDAL_LIBS)

noinst_LIBRARIES = libIntersectionLocator.a
libIntersectionLocator_a_SOURCES = Circle.cpp \
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

INCLUDES = $(GDAL_CFLAGS)

# This is placed here because it's required by the program in the 
# TrainClassifier subdirectory, and this tells automake to try to
# build it first before going there
BUILT_SOURCES = libIntersectionLocator.a

SUBDIRS = TrainClassifier
