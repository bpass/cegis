/**
 * @file ComplexQuadraticFormula.h
 * @author Austin Hartman
 * $Id: ComplexQuadraticFormula.h,v 1.1 2005/04/27 15:38:21 ahartman Exp $
 */

#ifndef AUSTIN_COMPLEXQUADRATICFORMULA_H
#define AUSTIN_COMPLEXQUADRATICFORMULA_H

#include <vector>
#include <complex>

#include "Polynomial.h"

template<class T>
class ComplexQuadraticFormula<std::complex<T> >
{
public:    
    typedef std::vector<std::complex<T> > Roots;
    Roots operator()(const Polynomial<std::complex<T> >& p) const;
};

#include "ComplexQuadraticFormula.hpp"

#endif
