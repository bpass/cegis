/**
 * @file PolynomialRootFinder.h
 * $Id: PolynomialRootFinder.h,v 1.1 2005/04/19 21:53:27 ahartman Exp $
 */

#ifndef AUSTIN_POLYNOMIALROOTFINDER_H
#define AUSTIN_POLYNOMIALROOTFINDER_H

#include "Polynomial.h"
#include "Muller.h"
#include "QuadraticFormula.h"

template<class T>
class PolynomialRootFinder
{
public:
    typedef std::vector<T> Roots;
    Roots operator()(const Polynomial<T>& p, const T& rootGuess = 0, 
                     const T& h = 1,
                     const T& epsilon = .0001, 
                     const size_t& maxIterations = 100) const;
};

#include "PolynomialRootFinder.hpp"

#endif
