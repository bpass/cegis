## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Mark Schisler 
#
# $Id: Makefile.am,v 1.6 2005/10/13 22:27:40 mschisler Exp $
#

# SUBDIRS = tests

OBJDIR = .obj

COMPILE = %OD

noinst_PROGRAMS = reproject slave

COMSOURCES = ClientSocket.cpp ProjImageFactory.cpp ProjImageIn.cpp ServerSocket.cpp \
  		     GeneralProjector.cpp  ProjImageInList.cpp \
		     SlaveProjector.cpp ProjectorFactory.cpp \
		     ProjImageOut.cpp  SocketWrapper.cpp \
		     ProjImageParams.cpp \
		     ProjImageData.cpp  ProjImageScale.cpp \
			 ProjImageOutPiece.cpp \
			 ProjProcessSpawner.cpp  ProjectorManager.cpp \
			 ImageStitcher.cpp ImageStitcherNode.cpp
			 
reproject_SOURCES = MasterMain.cpp $(COMSOURCES)
slave_SOURCES = SlaveMain.cpp $(COMSOURCES) 
					
AM_CPPFLAGS = -pg -g -Wno-deprecated -Wall -W -pedantic \
-Iinclude/Projector \
-I/usr/src/linux/include \
-Iinclude \
-Iinclude/ImageLib 

reproject_LDFLAGS = -pg 
slave_LDFLAGS = -pg

reproject_LDADD = \
	include/Projector/.obj/ProjectionParams.o \
	include/Projector/.obj/ProjectorException.o \
	include/Projector/.obj/ProjUtil.o \
	lib/JPEGImageOFile.o \
	lib/PNGImageOFile.o \
	-Llib \
	-L/usr/local/lib \
	-L/usr/lib \
	-lProjectionMesh -lMathLib -lProjectionIO -lImageLib \
	-lgeotiff -ltiff -lProjection -lMiscUtils \
	-lpthread -lgctpc -lboost_thread -lpng -ljpeg 

slave_LDADD = $(reproject_LDADD)
