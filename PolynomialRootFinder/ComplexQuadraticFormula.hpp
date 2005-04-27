/**
 * @file ComplexQuadraticFormula.hpp
 * @author Austin Hartman
 * $Id: ComplexQuadraticFormula.hpp,v 1.1 2005/04/27 15:38:21 ahartman Exp $
 */

template<class T>
typename QuadraticFormula<std::complex<T> >::Roots
QuadraticFormula<std::complex<T> >::operator()(const Polynomial<T>& p) const
{
    typename QuadraticFormula<std::complex<T> >::Roots roots;

    const std::complex<T>& a = p.getCoefficient(2);
    const std::complex<T>& b = p.getCoefficient(1);
    const std::complex<T>& c = p.getCoefficient(0);

    const std::complex<T> discriminant = 
        b*b - static_cast<std::complex<T> >(4)*a*c;
    const std::complex<T> twoC = static_cast<std::complex<T> >(2)*c;

    if(discriminant != static_cast<std::complex<T> >(0))
    {
        const std::complex<T> sqrtDiscriminant = std::sqrt(discriminant);
        const std::complex<T> denom1 = -b + sqrtDiscriminant;
        const std::complex<T> denom2 = -b - sqrtDiscriminant;
        if(denom1 != static_cast<std::complex<T> >(0))
        {
            roots.push_back( static_cast<std::complex<T> >(twoC / denom1) );
        }
        if(denom2 != static_cast<std::complex<T> >(0))
        {
            roots.push_back( static_cast<std::complex<T> >(twoC / denom2) );
        }
    }
    else
    {
        // calculate the single, repeated root
        roots.push_back( static_cast<std::complex<T> >(twoC / -b) );
    }

    return roots;
}

