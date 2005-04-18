template<class T>
typename QuadraticFormula<T>::Roots
QuadraticFormula<T>::operator()(const Polynomial<T>& p) const
{
    if(p.degree() != 2)
    {
        throw typename QuadraticFormula<T>::InvalidPolynomial;
    }

    typename QuadraticFormula<T>::Roots roots;

    const T& a = p.getCoefficient(2);
    const T& b = p.getCoefficient(1);
    const T& c = p.getCoefficient(0);

    const T discriminant = b*b - 4*a*c;

    // if the discriminant is less than 0, return a blank solution
    // because there are no real roots
    if(discriminant > 0)
    {
        // calculate the two roots
        const T sqrtDiscriminant = std::sqrt(discriminant)
        roots.push_back( T(2*c / (-b + sqrtDiscriminant)) );
        roots.push_back( T(2*c / (-b - sqrtDiscriminant)) );
    }
    else if(discriminant == 0)
    {
        // calculate the single, repeated root
        roots.push_back( T(2*c / -b) );
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

