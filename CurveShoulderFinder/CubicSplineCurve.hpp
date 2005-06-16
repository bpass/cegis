/**
 * @file CubicSplineCurve.hpp
 * @author Austin Hartman
 *
 * $Id: CubicSplineCurve.hpp,v 1.3 2005/06/16 23:08:34 ahartman Exp $
 */

#ifdef AUSTIN_CUBICSPLINECURVE_H
#ifndef AUSTIN_CUBICSPLINECURVE_HPP
#define AUSTIN_CUBICSPLINECURVE_HPP

template<class T>
CubicSplineCurve<T>::CubicSplineCurve()
{}

template<class T>
CubicSplineCurve<T>::
CubicSplineCurve(const typename FittingCurve<T>::Points& points)
{
    findSolution(points);        
}

template<class T>
void
CubicSplineCurve<T>::setPoints(const typename FittingCurve<T>::Points& points)
{
    findSolution(points);        
}

template<class T>
T
CubicSplineCurve<T>::operator()(const T& x) const
{
    return m_evaluator(m_solution, x);
}

template<class T>
CubicSplineCurve<T>::~CubicSplineCurve()
{}

template<class T>
MyVector<T>
CubicSplineCurve<T>::
getXCoords(const typename FittingCurve<T>::Points& points) const
{
    MyVector<T> rv(points.size());
    for(size_t i = 0; i < points.size(); ++i)
    {
        rv[i] = points[i].x();
    }
    return rv;
}

template<class T>
MyVector<T>
CubicSplineCurve<T>::
getYCoords(const typename FittingCurve<T>::Points& points) const
{
    MyVector<T> rv(points.size());
    for(size_t i = 0; i < points.size(); ++i)
    {
        rv[i] = points[i].y();
    }
    return rv;
}

template<class T>
void
CubicSplineCurve<T>::
findSolution(const typename FittingCurve<T>::Points& points)
{
    if(points.size() <= 1)
    {
        throw TooFewPoints();
    }

    CubicSplineSolver<T> solver;
    m_solution = solver(getXCoords(points), getYCoords(points));
}

#endif
#endif

