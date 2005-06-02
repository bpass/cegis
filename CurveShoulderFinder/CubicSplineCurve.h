#ifndef AUSTIN_CUBICSPLINECURVE_H
#define AUSTIN_CUBICSPLINECURVE_H

#include "FittingCurve.h"
#include "CubicSplineSolver.h"
#include "CubicSplineEvaluator.h"
#include "MyVector.h"

template<class T>
class CubicSplineCurve : public FittingCurve<T>
{
public:
    CubicSplineCurve();
    CubicSplineCurve(const typename FittingCurve<T>::Points& points);

    virtual void setPoints(const typename FittingCurve<T>::Points& points);
    virtual T operator()(const T& x) const;

    virtual ~CubicSplineCurve();

    class TooFewPoints
    {};

private:
    typename CubicSplineSolver<T>::Solution m_solution;
    CubicSplineEvaluator<T> m_evaluator;

    MyVector<T> getXCoords(const typename FittingCurve<T>::Points& 
                                          points) const;
    MyVector<T> getYCoords(const typename FittingCurve<T>::Points& 
                                          points) const;
    void findSolution(const typename FittingCurve<T>::Points& points);
};

#include "CubicSplineCurve.hpp"

#endif

