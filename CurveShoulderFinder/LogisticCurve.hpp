/**
 * @file LogisticCurve.hpp
 * @author Austin Hartman
 *
 * $Id: LogisticCurve.hpp,v 1.3 2005/06/13 23:09:25 ahartman Exp $
 */

#include <cmath>
#include "GaussianSolver.h"
#include "PartialPivotingGaussianSolver.h"

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
    typename NonlinearRegression<T>::Parameters p;
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
    using std::cout;

    typename NonlinearRegression<T>::Parameters initialGuesses;
    initialGuesses.reserve(numParameters);
    initialGuesses.push_back(m_a);
    initialGuesses.push_back(m_b);
    initialGuesses.push_back(m_c);

    typename NonlinearRegression<T>::FirstPartials partials;
    partials.reserve(numParameters);
    partials.push_back(firstPartialA);
    partials.push_back(firstPartialB);
    partials.push_back(firstPartialC);

    NonlinearRegression<T> nlr;
    typename NonlinearRegression<T>::Parameters parameters =
        nlr(m_points, initialGuesses, 
            logisticFunction, partials, stoppingCondition);
    m_a = parameters[0];
    m_b = parameters[1];
    m_c = parameters[2];

    cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ":\n"
         << "\ta = " << m_a << '\n'
         << "\tb = " << m_b << '\n'
         << "\tc = " << m_c << '\n';
}

template<class T>
T
LogisticCurve<T>::logisticFunction(const typename NonlinearRegression<T>::
                                                  Parameters& p,
                                   const T& x)
{
    return p[0]/(1+p[1]*std::exp(-p[2]*x));
}

template<class T>
T
LogisticCurve<T>::firstPartialA(const typename NonlinearRegression<T>::
                                               Parameters& p,
                                const T& x)
{
    return 1/(commonPartialDivisor(p, x));
}

template<class T>
T
LogisticCurve<T>::firstPartialB(const typename NonlinearRegression<T>::
                                               Parameters& p,
                                const T& x)
{
    const T cpd = commonPartialDivisor(p, x);
    return -p[0]*std::exp(-p[2]*x)/(cpd*cpd);
}

template<class T>
T
LogisticCurve<T>::firstPartialC(const typename NonlinearRegression<T>::
                                               Parameters& p,
                                const T& x)
{
    const T cpd = commonPartialDivisor(p, x);
    return p[0]*p[1]*x*std::exp(-p[2]*x)/(cpd*cpd);
}

template<class T>
T
LogisticCurve<T>::commonPartialDivisor(const typename NonlinearRegression<T>::
                                                      Parameters& p,
                                       const T& x)
{
    return 1+p[1]*std::exp(-p[2]*x);
}

template<class T>
bool
LogisticCurve<T>::stoppingCondition(const typename NonlinearRegression<T>::
                                                   Parameters& deltaA)
{
    const T epsilon = .0001;
    for(size_t i = 0; i < deltaA.size(); ++i)
    {
        if(std::abs(deltaA[i]) > epsilon)
        {
            return false;
        }
    }
    return true;
}

template<class T>
T 
LogisticCurve<T>::sumResiduals(const typename FittingCurve<T>::
                                              Points& points,
                               const typename NonlinearRegression<T>::
                                              Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        sum += points[i].y() - logisticFunction(p, points[i].x());
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsFirstPartialA(const typename FittingCurve<T>::Points& points,
                          const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        sum += -1 / (1+p[1]*std::exp(-p[2]*points[i].x()));
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsFirstPartialB(const typename FittingCurve<T>::Points& points,
                          const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        sum += (p[0]*expFactor) / (denomFactor*denomFactor);
               
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsFirstPartialC(const typename FittingCurve<T>::Points& points,
                          const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        sum += (-p[0]*p[1]*points[i].x()*expFactor) / (denomFactor*denomFactor);
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialAA(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    return 0;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialAB(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        sum += expFactor / (denomFactor*denomFactor);
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialAC(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        sum += -p[1]*points[i].x()*expFactor / (denomFactor*denomFactor);
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialBA(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    return sumResidualsSecondPartialAB(points, p);
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialBB(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        sum += (-2*p[0]*expFactor*expFactor) / 
               (denomFactor*denomFactor*denomFactor);
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialBC(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        const T firstTerm = (2*p[0]*expFactor*expFactor*p[1]*points[i].x()) /
                            (denomFactor*denomFactor*denomFactor);
        const T secondTerm = (-p[0]*points[i].x()*expFactor) /
                             (denomFactor*denomFactor);
        sum += firstTerm + secondTerm;
    }
    return sum;
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialCA(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    return sumResidualsSecondPartialAC(points, p);
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialCB(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    return sumResidualsSecondPartialBC(points, p);
}

template<class T>
T
LogisticCurve<T>::
sumResidualsSecondPartialCC(const typename FittingCurve<T>::Points& points,
                            const typename NonlinearRegression<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(-p[2]*points[i].x());
        const T denomFactor = 1+p[1]*expFactor;
        const T firstTerm = (-2*p[0]*p[1]*p[1]*points[i].x()*points[i].x()*
                             expFactor*expFactor) /
                            (denomFactor*denomFactor*denomFactor);
        const T secondTerm = (p[0]*p[1]*points[i].x()*points[i].x()*expFactor) / 
                             (denomFactor*denomFactor);
        sum += firstTerm + secondTerm;
    }
    return sum;
}

