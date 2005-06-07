#ifndef AUSTIN_NONLINEARREGRESSIONTEST_H
#define AUSTIN_NONLINEARREGRESSIONTEST_H

#include "NonlinearRegression.h"

void nonlinearRegressionTest();

template<class T>
T bookExampleFunction(const typename NonlinearRegression<T>::Parameters& p, 
                      const T& x);
template<class T>
T bookExampleFirstPartialA0(const typename NonlinearRegression<T>::Parameters& p, 
                            const T& x);
template<class T>
T bookExampleFirstPartialA1(const typename NonlinearRegression<T>::Parameters& p, 
                            const T& x);
template<class T>
bool bookExampleStoppingTest(const typename NonlinearRegression<T>::
                                            Parameters& parameterChanges);

#endif
