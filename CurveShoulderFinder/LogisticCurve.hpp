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

