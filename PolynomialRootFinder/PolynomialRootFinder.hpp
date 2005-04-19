/**
 * @file PolynomialRootFinder.hpp
 * @author Austin Hartman
 * $Id: PolynomialRootFinder.hpp,v 1.2 2005/04/19 22:01:24 ahartman Exp $
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
        cerr << __FILE__ << ':' << __LINE__ 
             << ": currentP == " << currentP << '\n';
        Muller<T> muller;
        currentRoot = muller(currentP, /*currentRoot*/rootGuess, h, epsilon, maxIterations);
        roots.push_back(currentRoot);
        currentP = currentP.deflate(currentRoot);
    }
    cerr << __FILE__ << ':' << __LINE__ 
         << ": roots.size() == " << roots.size() << '\n';
    
    if(currentP.degree() <= 2 && currentP.degree() >= 1)
    {
        cerr << __FILE__ << ':' << __LINE__ 
             << ": currentP == " << currentP << '\n';
        QuadraticFormula<T> quadf;
        typename QuadraticFormula<T>::Roots quadRoots = quadf(currentP);
        cerr << __FILE__ << ':' << __LINE__
             << ": quadRoots.size() == " << quadRoots.size() << '\n';
        roots.insert(roots.end(), quadRoots.begin(), quadRoots.end());
    }
    cerr << __FILE__ << ':' << __LINE__ 
         << ": roots.size() == " << roots.size() << '\n';
    
    return roots;
}

