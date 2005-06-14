/**
 * @file MarquardtMethod.hpp
 * @author Austin Hartman
 *
 * $Id: MarquardtMethod.hpp,v 1.1 2005/06/14 23:40:51 ahartman Exp $
 */

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include "DiagonalMatrix.h"
#include "InvertMatrix.h"

template<class T>
const size_t MarquardtMethod<T>::m_maxIterations = 100;

template<class T>
typename MarquardtMethod<T>::Parameters
MarquardtMethod<T>::operator()(
    const typename MarquardtMethod<T>::Points& points,
    const typename MarquardtMethod<T>::Parameters& initialGuesses,
    const typename MarquardtMethod<T>::SumResiduals& sumResiduals,
    const typename MarquardtMethod<T>::FirstPartials& firstPartials,
    const typename MarquardtMethod<T>::SecondPartials& secondPartials,
    const typename MarquardtMethod<T>::StoppingTest& stoppingTest,
    const size_t maxIterations) const
{
#ifdef DEBUG_PRINT
    using std::cout;
    using std::fixed;
    using std::setw;
    cout.precision(8);
    cout << fixed;
    const size_t width = cout.precision() + 4;
#endif

    // first, check to make sure everything is of the right size
    const size_t numParameters = initialGuesses.size();
    if(firstPartials.size() != numParameters)
    {
        throw typename MarquardtMethod<T>::InvalidFirstPartialsSize();
    }
    if(secondPartials.size() != numParameters * numParameters)
    {
        throw typename MarquardtMethod<T>::InvalidSecondPartialsSize();
    }

    bool done = false;
    size_t iterations = 0;
    typename MarquardtMethod<T>::Parameters parameters(initialGuesses);
    T lambda = 1000;
    const T lambdaFactor = 10;
    while(iterations < maxIterations && !done)
    {
        // create the Hessian matrix
        DenseMatrix<T> H(numParameters, numParameters);
        for(size_t j = 0; j < numParameters; ++j)
        {
            for(size_t i = 0; i < numParameters; ++i)
            {
                H[i][j] = secondPartials[numParameters*i+j](points, parameters);
            }
        }
        
        // take the diagonal of the Hessian and multiply it by lambda
        DiagonalMatrix<T> diagH(numParameters);
        for(size_t i = 0; i < numParameters; ++i)
        {
            diagH[i][i] = lambda * H[i][i];
        }

        // sum the Hessian and the diagonal matrix and take that matrix's
        // inverse
        DenseMatrix<T> sum = H + diagH;
        InvertMatrix<T> inverter;
        DenseMatrix<T> inverse = inverter(sum);

        // calculate the gradient of the function
        MyVector<T> delF(numParameters);
        for(size_t i = 0; i < numParameters; ++i)
        {
            delF[i] = firstPartials[i](points, parameters);
        }

        // calculate the potential new parameters
        MyVector<T> product = inverse * delF;
        typename MarquardtMethod<T>::Parameters newParameters(numParameters);
        for(size_t i = 0; i < numParameters; ++i)
        {
            newParameters[i] = parameters[i] - product[i];
        }

        const T oldSumResiduals = sumResiduals(points, parameters);
        const T newSumResiduals = sumResiduals(points, newParameters);

        // if the parameter estimates have improved
        if(newSumResiduals < oldSumResiduals)
        {
            parameters = newParameters;
            lambda /= lambdaFactor;
            done = stoppingTest(points, parameters);
        }
        // if the parameter estimates are worse than before
        else
        {
            // don't use the new parameters
            lambda *= lambdaFactor;
        }

        ++iterations;

#ifdef DEBUG_PRINT
        cout << "Sum Residuals[" << iterations << "] = " 
             << setw(width) << sumResiduals(points, parameters) << '\n';
#endif
    }

    return parameters;
}

