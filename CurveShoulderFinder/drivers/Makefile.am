## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.3 2005/10/26 19:03:59 ahartman Exp $
#

noinst_PROGRAMS = ComparePrecipitation LittleRiver

ComparePrecipitation_SOURCES = ComparePrecipitation.cpp
ComparePrecipitation_CPPFLAGS = $(GDAL_CFLAGS) -I..
ComparePrecipitation_LDADD = $(GDAL_LIBS)

LittleRiver_SOURCES = LittleRiver.cpp
LittleRiver_CPPFLAGS = $(GDAL_CFLAGS) -I..
LittleRiver_LDADD = $(GDAL_LIBS)

