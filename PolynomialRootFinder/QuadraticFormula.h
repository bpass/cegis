#ifndef AUSTIN_QUADRATICFORMULA_H
#define AUSTIN_QUADRATICFORMULA_H

#include "Polynomial.h"

template<class T>
class QuadraticFormula
{
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

