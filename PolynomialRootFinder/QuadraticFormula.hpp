/**
 * @file QuadraticFormula.hpp
 * @author Austin Hartman
 * $Id: QuadraticFormula.hpp,v 1.5 2005/04/22 16:54:36 ahartman Exp $
 */

template<class T>
typename QuadraticFormula<T>::Roots
QuadraticFormula<T>::operator()(const Polynomial<T>& p) const
{
    typename QuadraticFormula<T>::Roots roots;

    const T& a = p.getCoefficient(2);
    const T& b = p.getCoefficient(1);
    const T& c = p.getCoefficient(0);

    const T discriminant = b*b - static_cast<T>(4)*a*c;

    if(discriminant > static_cast<T>(0))
    {
        // calculate the two roots or the single root (if a == 0)
        const T sqrtDiscriminant = std::sqrt(discriminant);

        const T denom1 = -b + sqrtDiscriminant;
        const T denom2 = -b - sqrtDiscriminant;

        if(denom1 != static_cast<T>(0))
        {
            roots.push_back( T(static_cast<T>(2)*c / denom1) );
        }
        if(denom2 != static_cast<T>(0))
        {
            roots.push_back( T(static_cast<T>(2)*c / denom2) );
        }
    }
    else if(discriminant == static_cast<T>(0))
    {
        // calculate the single, repeated root
        roots.push_back( T(static_cast<T>(2)*c / -b) );
    }
    // if the discriminant is less than 0, return a blank vector because
    // there are no real roots

    return roots;
}

template<class T>
const char*
QuadraticFormula<T>::InvalidPolynomial::what() const
{
    return "Invalid polynomial for quadratic formula";
}

