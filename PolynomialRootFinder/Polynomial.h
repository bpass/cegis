/**
 * @file Polynomial.h
 * @author Austin Hartman
 * $Id: Polynomial.h,v 1.4 2005/04/26 20:53:47 ahartman Exp $
 */

#ifndef AUSTIN_POLYNOMIAL_H
#define AUSTIN_POLYNOMIAL_H

#include <vector>
#include <algorithm>
#include <iostream>

using std::ostream;
using std::ofstream;
using std::istream;

template<class T>
class Polynomial
{
public:
    class Term;

    Polynomial();
    Polynomial(const T& value, const size_t& coeff);

    Polynomial<T>& operator+=(const Polynomial<T>& rhs);
    friend Polynomial<T> operator+ <>(const Polynomial<T>& lhs, 
                                      const Polynomial<T>& rhs);

    Polynomial<T> operator-() const;

    Polynomial<T>& operator-=(const Polynomial<T>& rhs);
    friend Polynomial<T> operator- <>(const Polynomial<T>& lhs, 
                                      const Polynomial<T>& rhs);

    Polynomial<T>& operator*=(const T& scalar);
    friend Polynomial<T> operator* <>(const Polynomial<T>& p,
                                      const T& scalar);
    friend Polynomial<T> operator* <>(const T& scalar,
                                      const Polynomial<T>& p);

    const T& getCoefficient(const size_t& power) const;
    
    size_t degree() const;
    Polynomial<T> deflate(const T& root) const;
    Polynomial<T> derivative() const;

    T operator()(const T& x) const;

    friend ostream& operator<< <>(ostream& os, const Polynomial<T>& p);
    friend ofstream& operator<< <>(ofstream& ofs, const Polynomial<T>& p);
    friend istream& operator>> <>(istream& is, Polynomial<T>& p);

    class Term
    {
    public:
        Term(const T& coeff, const size_t& pow);

        const size_t& power() const;

        T& coefficient();
        const T& coefficient() const;

    private:
        size_t power_;
        T coefficient_;
    };

private:
    static const char letter;
    static const T constZeroCoefficient;
    
    std::vector<Term> terms;

    void removeZeroCoefficients();

    class ZeroCoefficient : public std::unary_function<Term, bool>
    {
    public:
        bool operator()(const Term& t) const;
    };

    class LowerPower : public std::binary_function<Term, Term, bool>
    {
    public:
        bool operator()(const Term& t1, const Term& t2) const;
    };

    class EqualPowers : public std::binary_function<Term, Term, bool>
    {
    public:
        bool operator()(const Term& t1, const Term& t2) const;
    };
};

#include "Polynomial.hpp"

#endif

