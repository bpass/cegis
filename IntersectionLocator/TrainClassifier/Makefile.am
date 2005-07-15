## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.2 2005/07/15 01:09:55 ahartman Exp $
#

noinst_PROGRAMS = TrainClassifier
TrainClassifier_SOURCES = main.cpp
TrainClassifier_LDADD = $(GDAL_LIBS) -L.. -lIntersectionLocator
TrainClassifier_DEPENDENCIES = ../libIntersectionLocator.la
INCLUDES = $(GDAL_CFLAGS) -I..
