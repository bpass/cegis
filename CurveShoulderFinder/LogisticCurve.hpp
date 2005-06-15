/**
 * @file LogisticCurve.hpp
 * @author Austin Hartman
 *
 * $Id: LogisticCurve.hpp,v 1.5 2005/06/15 01:47:44 ahartman Exp $
 */

#include <cmath>

#include "GaussianSolver.h"
#include "PartialPivotingGaussianSolver.h"

//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <iostream>
#endif

template<class T>
const size_t LogisticCurve<T>::numParameters = 3; 

template<class T>
LogisticCurve<T>::
LogisticCurve<T>(const typename FittingCurve<T>::Points& points, 
                 const T& a, const T& b, const T& c)
    : m_a(a), m_b(b), m_c(c), m_points(points)
{
    findSolution();
}

template<class T>
LogisticCurve<T>::~LogisticCurve()
{}

template<class T>
void
LogisticCurve<T>::setPoints(const typename FittingCurve<T>::Points& points)
{
    m_points = points;
    findSolution();
}

template<class T>
void
LogisticCurve<T>::setInitialGuesses(const T& a, const T& b, const T& c)
{
    m_a = a;
    m_b = b;
    m_c = c;
    findSolution();
}

template<class T>
T
LogisticCurve<T>::operator()(const T& x) const
{
    typename MarquardtMethod<T>::Parameters p;
    p.reserve(numParameters);
    p.push_back(m_a);
    p.push_back(m_b);
    p.push_back(m_c);
    return logisticFunction(p, x);
//    return m_a/(1+m_b*std::exp(-m_c*x));
}

template<class T>
void
LogisticCurve<T>::findSolution()
{
#ifdef DEBUG_PRINT
    using std::cout;
#endif

    typename MarquardtMethod<T>::Parameters initialGuesses;
    initialGuesses.reserve(numParameters);
    initialGuesses.push_back(m_a);
    initialGuesses.push_back(m_b);
    initialGuesses.push_back(m_c);

    typename MarquardtMethod<T>::FirstPartials firstPartials;
    firstPartials.reserve(numParameters);
    firstPartials.push_back(sumResidualsFirstPartialA);
    firstPartials.push_back(sumResidualsFirstPartialB);
    firstPartials.push_back(sumResidualsFirstPartialC);

    typename MarquardtMethod<T>::SecondPartials secondPartials;
    secondPartials.reserve(numParameters * numParameters); 
    secondPartials.push_back(sumResidualsSecondPartialAA);
    secondPartials.push_back(sumResidualsSecondPartialAB);
    secondPartials.push_back(sumResidualsSecondPartialAC);
    secondPartials.push_back(sumResidualsSecondPartialBA);
    secondPartials.push_back(sumResidualsSecondPartialBB);
    secondPartials.push_back(sumResidualsSecondPartialBC);
    secondPartials.push_back(sumResidualsSecondPartialCA);
    secondPartials.push_back(sumResidualsSecondPartialCB);
    secondPartials.push_back(sumResidualsSecondPartialCC);

    MarquardtMethod<T> mm;
    typename MarquardtMethod<T>::Parameters parameters = 
        mm(m_points, initialGuesses, 
           sumResiduals, firstPartials, secondPartials, 
           stoppingCondition, 1000);
    
    m_a = parameters[0];
    m_b = parameters[1];
    m_c = parameters[2];

#ifdef DEBUG_PRINT
    cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ":\n"
         << "\ta = " << m_a << '\n'
         << "\tb = " << m_b << '\n'
         << "\tc = " << m_c << '\n';
#endif
}

template<class T>
T
LogisticCurve<T>::logisticFunction(const typename MarquardtMethod<T>::
                                                  Parameters& p,
                                   const T& x)
{
    return p[0]/(1+p[1]*std::exp(-p[2]*x));
}

template<class T>
T
LogisticCurve<T>::firstPartialA(const typename MarquardtMethod<T>::
                                               Parameters& p,
                                const T& x)
{
    return 1/(commonPartialDivisor(p, x));
}

template<class T>
T
LogisticCurve<T>::firstPartialB(const typename MarquardtMethod<T>::
                                               Parameters& p,
                                const T& x)
{
    const T cpd = commonPartialDivisor(p, x);
    return -p[0]*std::exp(-p[2]*x)/(cpd*cpd);
}

template<class T>
T
LogisticCurve<T>::firstPartialC(const typename MarquardtMethod<T>::
                                               Parameters& p,
                                const T& x)
{
    const T cpd = commonPartialDivisor(p, x);
    return p[0]*p[1]*x*std::exp(-p[2]*x)/(cpd*cpd);
}

template<class T>
T
LogisticCurve<T>::commonPartialDivisor(const typename MarquardtMethod<T>::
                                                      Parameters& p,
                                       const T& x)
{
    return 1+p[1]*std::exp(-p[2]*x);
}

template<class T>
T 
LogisticCurve<T>::
sumResiduals(const typename FittingCurve<T>::Points& points,
             const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T factor = points[i].y() - logisticFunction(p, points[i].x());
        sum += factor * factor;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsFirstPartialA(const typename FittingCurve<T>::Points& points,
                          const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += -2 * (points[i].y() - p[0] / term) / term;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsFirstPartialB(const typename FittingCurve<T>::Points& points,
                          const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += 2 * (points[i].y() - p[0] / term) * 
               (p[0] / (term * term)) * expFactor;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsFirstPartialC(const typename FittingCurve<T>::Points& points,
                          const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += -2 * (points[i].y() - p[0] / term) *
               p[0] / (term * term) * p[1] * points[i].x() *
               expFactor;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialAA(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += 2 / (term * term);
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialAB(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += -2*p[0]*expFactor / (term * term * term);
        sum += 2*(points[i].y() - p[0] / term) / (term * term) * expFactor;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialAC(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += 2 * p[0] * p[1] * points[i].x() * expFactor / 
               (term * term * term);
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialBA(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    return sumResidualsSecondPartialAB(points, p);
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialBB(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += 2 * p[0] * p[0] * expFactor * expFactor /
               (term * term * term * term);
        sum += -4 * (points[i].y() - p[0] / term) * 
               (p[0] / (term * term * term)) * 
               expFactor * expFactor;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialBC(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += -2 * p[0] * p[0] * p[1] * points[i].x() * 
               expFactor * expFactor /
               (term * term * term * term);
        sum += 4 * (points[i].y() - p[0] / term) *
               p[0] / (term * term * term) * 
               expFactor * expFactor * p[1] * points[i].x();
        sum += -2 * (points[i].y() - p[0] / term) *
               p[0] / (term * term) * points[i].x() * expFactor;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialCA(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    return sumResidualsSecondPartialAC(points, p);
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialCB(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    return sumResidualsSecondPartialBC(points, p);
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialCC(const typename FittingCurve<T>::Points& points,
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T term = 1 + p[1]*expFactor;
        sum += 2 * p[0] * p[0] * p[1] * p[1] * points[i].x() * points[i].x() * 
               expFactor * expFactor / (term * term * term * term);
        sum += -4 * (points[i].y() - p[0] / term) * 
               (p[0] / (term * term * term)) * p[1] * p[1] *
               points[i].x() * points[i].x() * expFactor * expFactor;
        sum += 2 * (points[i].y() - p[0] / term) *
               (p[0] / (term * term)) * p[1] *
               points[i].x() * points[i].x() * expFactor;
    }
    return sum;
}

template<class T>
bool
LogisticCurve<T>::
stoppingCondition(const typename FittingCurve<T>::Points& points,
                  const typename MarquardtMethod<T>::Parameters& p)
{
    const T tolerance = .0001;
    if(sumResiduals(points, p) > tolerance)
    {
        return false;
    }
    return true;
}

