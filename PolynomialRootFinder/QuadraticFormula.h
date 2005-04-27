/**
 * @file QuadraticFormula.h
 * @author Austin Hartman
 * $Id: QuadraticFormula.h,v 1.3 2005/04/27 15:37:29 ahartman Exp $
 */

#ifndef AUSTIN_QUADRATICFORMULA_H
#define AUSTIN_QUADRATICFORMULA_H

#include <vector>

#include "Polynomial.h"

template<class T>
class QuadraticFormula
{
public:
    typedef std::vector<T> Roots;
    Roots operator()(const Polynomial<T>& p) const;
};

#include "QuadraticFormula.hpp"

#endif

