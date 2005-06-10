/**
 * @file CubicSplineCurve.hpp
 * @author Austin Hartman
 *
 * $Id: CubicSplineCurve.hpp,v 1.2 2005/06/10 21:52:23 ahartman Exp $
 */

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

