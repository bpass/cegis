/**
 * @file LogisticCurve.h
 * @author Austin Hartman
 *
 * $Id: LogisticCurve.h,v 1.3 2005/06/13 23:09:25 ahartman Exp $
 */

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
    static T logisticFunction(
            const typename NonlinearRegression<T>::Parameters& p, 
            const T& x);
    static T firstPartialA(
            const typename NonlinearRegression<T>::Parameters& p, 
            const T& x);
    static T firstPartialB(
            const typename NonlinearRegression<T>::Parameters& p, 
            const T& x);
    static T firstPartialC(
            const typename NonlinearRegression<T>::Parameters& p, 
            const T& x);
    static T commonPartialDivisor(
            const typename NonlinearRegression<T>::Parameters& p, 
            const T& x);
    static bool stoppingCondition(
            const typename NonlinearRegression<T>::Parameters& deltaA);

    static T sumResiduals(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);

    static T sumResidualsFirstPartialA(
            const typename FittingCurve<T>::Points& points,
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsFirstPartialB(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsFirstPartialC(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);

    static T sumResidualsSecondPartialAA(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialAB(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialAC(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialBA(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialBB(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialBC(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialCA(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialCB(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
    static T sumResidualsSecondPartialCC(
            const typename FittingCurve<T>::Points& points, 
            const typename NonlinearRegression<T>::Parameters& p);
};

#include "LogisticCurve.hpp"

#endif

