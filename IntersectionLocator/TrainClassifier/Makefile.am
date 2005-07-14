## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.1 2005/07/14 00:29:32 ahartman Exp $
#

noinst_PROGRAMS = TrainClassifier
TrainClassifier_SOURCES = main.cpp
TrainClassifier_LDADD = $(GDAL_LIBS) -L.. -lIntersectionLocator
TrainClassifier_DEPENDENCIES = ../libIntersectionLocator.a
INCLUDES = $(GDAL_CFLAGS) -I..
