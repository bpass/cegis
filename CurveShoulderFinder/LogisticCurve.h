#ifndef AUSTIN_LOGISTICCURVE_H
#define AUSTIN_LOGISTICCURVE_H

#include "FittingCurve.h"

template<class T>
class LogisticCurve : public FittingCurve<T>
{
public:
    LogisticCurve();
    LogisticCurve(const typename FittingCurve<T>::Points& points, const T& L = 1);

    virtual void setPoints(const typename FittingCurve<T>::Points& points);
    virtual void setL(const T& L);
    virtual T operator()(const T& x) const;

    virtual ~LogisticCurve();

private:
    T m_c, m_a;
    T m_L;
    typename FittingCurve<T>::Points m_points;
    void findSolution();
};

#include "LogisticCurve.hpp"

#endif

