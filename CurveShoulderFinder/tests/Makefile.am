## Process this file with automake to produce Makefile.in
#
# @file Makefile.am
# @author Austin Hartman
#
# $Id: Makefile.am,v 1.10 2005/09/19 17:48:07 ahartman Exp $
#

AM_CPPFLAGS = -I..

TESTS = BSQReader_t \
        DiagonalMatrix_t \
        ImgReader_t \
        InvertMatrix_t \
        LUDecomposition_t \
        LUDecompositionSolver_t \
        MarquardtMethod_t \
        NonlinearRegression_t \
        PolynomialRegression_t \
        PolynomialCurve_t

noinst_PROGRAMS = $(TESTS)

BSQReader_t_SOURCES = BSQReader_t.cpp
DiagonalMatrix_t_SOURCES = DiagonalMatrix_t.cpp
ImgReader_t_SOURCES = ImgReader_t.cpp
ImgReader_t_CPPFLAGS = -I/usr/local/include -I..
ImgReader_t_LDADD = -L/usr/local/lib -lgdal
InvertMatrix_t_SOURCES = InvertMatrix_t.cpp
LUDecomposition_t_SOURCES = LUDecomposition_t.cpp
LUDecompositionSolver_t_SOURCES = LUDecompositionSolver_t.cpp
MarquardtMethod_t_SOURCES = MarquardtMethod_t.cpp
NonlinearRegression_t_SOURCES = NonlinearRegression_t.cpp
PolynomialRegression_t_SOURCES = PolynomialRegression_t.cpp
PolynomialCurve_t_SOURCES = PolynomialCurve_t.cpp

