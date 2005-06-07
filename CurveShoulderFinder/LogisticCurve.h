#ifndef AUSTIN_LOGISTICCURVE_H
#define AUSTIN_LOGISTICCURVE_H

#include "FittingCurve.h"
#include "MyVector.h"
#include "DenseMatrix.h"
#include "NonlinearRegression.h"

template<class T>
class LogisticCurve : public FittingCurve<T>
{
public:
    LogisticCurve(const typename FittingCurve<T>::Points& points, 
                  const T& aGuess, const T& bGuess, const T& cGuess);

    virtual void setPoints(const typename FittingCurve<T>::Points& points);
    virtual void setInitialGuesses(const T& aGuess, 
                                   const T& bGuess, 
                                   const T& cGuess);
    virtual T operator()(const T& x) const;

    virtual ~LogisticCurve();

private:
    T m_a, m_b, m_c;
    static const size_t numParameters;
    typename FittingCurve<T>::Points m_points;
    void findSolution();
    static T logisticFunction(const typename NonlinearRegression<T>::Parameters& p,
                              const T& x);
    static T firstPartialA(const typename NonlinearRegression<T>::Parameters& p,
                           const T& x);
    static T firstPartialB(const typename NonlinearRegression<T>::Parameters& p,
                           const T& x);
    static T firstPartialC(const typename NonlinearRegression<T>::Parameters& p,
                           const T& x);
    static T commonPartialDivisor(const typename NonlinearRegression<T>::
                                                 Parameters& p,
                                  const T& x);
    static bool stoppingCondition(const typename NonlinearRegression<T>::
                                                 Parameters& deltaA);
};

#include "LogisticCurve.hpp"

#endif

