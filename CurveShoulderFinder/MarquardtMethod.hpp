/**
 * @file MarquardtMethod.hpp
 * @author Austin Hartman
 *
 * $Id: MarquardtMethod.hpp,v 1.3 2005/06/15 19:56:40 ahartman Exp $
 */

#include "DiagonalMatrix.h"
#include "InvertMatrix.h"

//#define DEBUG_PRINT
//#define PRINT_SUMRESIDUALS

#if defined(DEBUG_PRINT) || defined(PRINT_SUMRESIDUALS)
#include <iostream>
#endif

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
#if defined(DEBUG_PRINT) || defined(PRINT_SUMRESIDUALS)
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
    T lambda = 100000;
    const T lambdaFactor = 10;
    size_t timesNotImproved = 0;
    while(iterations < maxIterations && !done && timesNotImproved < 5)
    {
        try
        {
            // create the Hessian matrix
            DenseMatrix<T> H(numParameters, numParameters);
            for(size_t j = 0; j < numParameters; ++j)
            {
                for(size_t i = 0; i < numParameters; ++i)
                {
                    H[i][j] = 
                        secondPartials[numParameters*i+j](points, parameters);
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
            typename MarquardtMethod<T>::Parameters 
                newParameters(numParameters);
            for(size_t i = 0; i < numParameters; ++i)
            {
                newParameters[i] = parameters[i] - product[i];
            }

            const T oldSumResiduals = sumResiduals(points, parameters);
            const T newSumResiduals = sumResiduals(points, newParameters);

#ifdef DEBUG_PRINT
//            cout << "lambda[" << iterations << "] = " << lambda << '\n';
#endif

            // if the parameter estimates have improved
            if(newSumResiduals < oldSumResiduals)
            {
                parameters = newParameters;
                lambda /= lambdaFactor;
                timesNotImproved = 0;
                done = stoppingTest(points, parameters);
            }
            // if the parameter estimates are worse than before
            else
            {
                // don't use the new parameters
                lambda *= lambdaFactor;
                ++timesNotImproved;
            }
        }
        catch(typename InvertMatrix<T>::MatrixIsSingular)
        {
            done = true;
        }

        ++iterations;

#ifdef DEBUG_PRINT
        cout << "Sum Residuals[" << iterations << "] = " 
             << setw(width) << sumResiduals(points, parameters) << '\n';
#endif
    }

#ifdef PRINT_SUMRESIDUALS
    cout << "Sum Residuals = " << sumResiduals(points, parameters) << '\n';
#endif

    return parameters;
}

#ifdef DEBUG_PRINT
#undef DEBUG_PRINT
#endif

#ifdef PRINT_SUMRESIDUALS
#undef PRINT_SUMRESIDUALS
#endif
