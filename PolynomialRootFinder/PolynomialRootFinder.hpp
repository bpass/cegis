/**
 * @file PolynomialRootFinder.hpp
 * @author Austin Hartman
 * $Id: PolynomialRootFinder.hpp,v 1.3 2005/04/22 16:57:17 ahartman Exp $
 */

template<class T>
typename PolynomialRootFinder<T>::Roots
PolynomialRootFinder<T>::
operator()(const Polynomial<T>& p, const T& rootGuess, const T& h,
           const T& epsilon, const size_t& maxIterations) const
{
    typename PolynomialRootFinder<T>::Roots roots;
    roots.reserve(p.degree());
    
    Polynomial<T> currentP(p);
    T currentRoot(rootGuess);
    
    while(currentP.degree() > 2)
    {
        Muller<T> muller;
        currentRoot = muller(currentP, /*currentRoot*/rootGuess, h, epsilon, maxIterations);
        roots.push_back(currentRoot);
        currentP = currentP.deflate(currentRoot);
    }
    
    if(currentP.degree() <= 2 && currentP.degree() >= 1)
    {
        QuadraticFormula<T> quadf;
        typename QuadraticFormula<T>::Roots quadRoots = quadf(currentP);
        roots.insert(roots.end(), quadRoots.begin(), quadRoots.end());
    }
    
    return roots;
}

