## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.1 2005/07/27 17:16:37 mschisler Exp $
#

# SUBDIRS = tests

OBJDIR = .obj

COMPILE = %OD

noinst_PROGRAMS = reproject

reproject_SOURCES = driver.cpp ProjImageFactory.cpp ProjImageOut.cpp \
FromMultiGeoProjector.cpp  ProjImageIn.cpp ProjImageParams.cpp \
ProjImageData.cpp ProjImageInList.cpp

AM_CPPFLAGS = -pg -g -Wno-deprecated -Wall -W -pedantic \
-I/work/mosixFastProjections/include/Projector \
-I/usr/src/linux/include \
-I/work/mosixFastProjections/include \
-I/work/mosixFastProjections/ImageLib \
-I/work/mosixFastProjections/


reproject_LDFLAGS = -pg 

reproject_LDADD = \
/work/mosixFastProjections/include/Projector/.obj/ProjectionParams.o \
/work/mosixFastProjections/include/Projector/.obj/ProjectorException.o \
/work/mosixFastProjections/include/Projector/.obj/ProjUtil.o \
/work/mosixFastProjections/.obj/JPEGImageOFile.o \
/work/mosixFastProjections/.obj/PNGImageOFile.o \
-L/work/mosixFastProjections/lib \
-L/usr/local/lib \
-L/usr/lib \
-lProjectionMesh -lMathLib -lProjectionIO -lImageLib \
-lgeotiff -ltiff -lProjection -lMiscUtils \
-lpthread -lgctpc -lboost_thread -lpng -ljpeg 
