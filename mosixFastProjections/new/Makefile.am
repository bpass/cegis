## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Mark Schisler 
#
# $Id: Makefile.am,v 1.3 2005/08/17 20:56:35 mschisler Exp $
#

# SUBDIRS = tests

OBJDIR = .obj

COMPILE = %OD

noinst_PROGRAMS = reproject

reproject_SOURCES =ClientSocket.cpp ProjImageFactory.cpp \
				   driver.cpp \
				   ProjImageIn.cpp ServerSocket.cpp \
				   FromMultiGeoProjector.cpp  ProjImageInList.cpp \
				   SlaveProjector.cpp ProjectorFactory.cpp \
				   ProjImageOut.cpp  SocketWrapper.cpp \
				   ProjImageParams.cpp \
                   ProjImageData.cpp  ProjImageScale.cpp \
				   ProjImageOutPiece.h
                   # ProjProcessSpawner.cpp  ProjectorSplitter.cpp
				    
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
