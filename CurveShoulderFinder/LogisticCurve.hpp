#include <cmath>
#include "LinearRegression.h"

template<class T>
LogisticCurve<T>::LogisticCurve()
{}

template<class T>
LogisticCurve<T>::
LogisticCurve(const typename FittingCurve<T>::Points& points, const T& L)
    : m_L(L), m_points(points)
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
LogisticCurve<T>::setL(const T& L)
{
    m_L = L;
    findSolution();
}

template<class T>
T
LogisticCurve<T>::operator()(const T& x) const
{
    return m_L / (1 + m_c * std::exp(m_a * x));
}

template<class T>
void
LogisticCurve<T>::findSolution()
{
    typename FittingCurve<T>::Points transformedPoints;
    transformedPoints.reserve(m_points.size());

    for(typename FittingCurve<T>::Points::iterator i = m_points.begin();
        i != m_points.end(); ++i)
    {
        transformedPoints.push_back(Point2D<T>(i->x(), 
                                               std::log(m_L/i->y() - 1)));
    }

    LinearRegression<T> lr;
    typename LinearRegression<T>::Solution lrSolution = lr(transformedPoints);

    m_c = std::exp(lrSolution.a0());
    m_a = lrSolution.a1();
}
