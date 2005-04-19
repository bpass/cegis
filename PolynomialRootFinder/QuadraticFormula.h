/**
 * @file QuadraticFormula.h
 * @author Austin Hartman
 * $Id: QuadraticFormula.h,v 1.2 2005/04/19 21:50:27 ahartman Exp $
 */

#ifndef AUSTIN_QUADRATICFORMULA_H
#define AUSTIN_QUADRATICFORMULA_H

#include <exception>

#include "Polynomial.h"

template<class T>
class QuadraticFormula
{
public:
    typedef std::vector<T> Roots;
    Roots operator()(const Polynomial<T>& p) const;

    class InvalidPolynomial : public std::exception
    {
    public:
        virtual const char* what() const;
    };
};

#include "QuadraticFormula.hpp"

#endif

