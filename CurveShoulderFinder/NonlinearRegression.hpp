/**
 * @file NonlinearRegression.hpp
 * @author Austin Hartman
 *
 * $Id: NonlinearRegression.hpp,v 1.2 2005/06/13 17:34:34 ahartman Exp $
 */

#include <cmath>
#include "GaussianSolver.h"
#include "PartialPivotingGaussianSolver.h"

template<class T>
const size_t NonlinearRegression<T>::m_maxIterations = 100;

template<class T>
typename NonlinearRegression<T>::Parameters
NonlinearRegression<T>::
operator()(const typename NonlinearRegression<T>::Points& points, 
           const typename NonlinearRegression<T>::Parameters& initialGuesses,
           const typename NonlinearRegression<T>::Function& f, 
           const typename NonlinearRegression<T>::FirstPartials& partials,
           const typename NonlinearRegression<T>::StoppingTest& stoppingTest,
           const size_t maxIterations) 
const
{
    using std::cout;

    if(initialGuesses.size() != partials.size())
    {
        throw typename NonlinearRegression<T>::
                       DifferentParametersPartialsSizes();
    }

    if(points.size() < partials.size())
    {
        throw typename NonlinearRegression<T>::NotEnoughPoints();
    }

    typename NonlinearRegression<T>::Parameters parameters(initialGuesses);
    bool done = false;
    size_t iterations = 0;

    while(iterations < maxIterations && !done)
    {
        const DenseMatrix<T> Z = createZMatrix(points, partials, parameters);
//        cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ": "
//             << "Z[" << iterations << "] =\n" << Z;
        
        const DenseMatrix<T> ZT = Z.transpose();
//        cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ": "
//             << "ZT[" << iterations << "] =\n" << ZT;
        
        const DenseMatrix<T> ZTZ = ZT*Z;
//        cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ": "
//             << "ZT*Z[" << iterations << "] =\n" << ZTZ;
        
        const MyVector<T> D = createDVector(points, f, parameters);
//        cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ": "
//             << "D[" << iterations << "] =\n" << DenseMatrix<T>(D);
        
        const MyVector<T> ZTD = ZT*D;
//        cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ": "
//             << "ZT*D[" << iterations << "] =\n" << DenseMatrix<T>(ZTD);

        const PartialPivotingGaussianSolver<T> gaussianSolver;
        const typename GaussianSolver<T>::Solution solution = 
            gaussianSolver(ZTZ, ZTD);
//        cout << __FILE__ << ':' << __LINE__ << ':' << __func__ << ": "
//             << "solution.matrix() =\n" << solution.matrix()
//             << "solution.vector() =\n" << DenseMatrix<T>(solution.vector());

        typename NonlinearRegression<T>::Parameters
            deltaA(solution.vector().getSize());
        for(size_t i = 0; i < parameters.size(); ++i)
        {
            deltaA[i] = solution.vector()[i];
            parameters[i] += deltaA[i];
        }

        done = stoppingTest(deltaA);
        ++iterations;
    }

    return parameters;
}

template<class T>
DenseMatrix<T>
NonlinearRegression<T>::
createZMatrix(const typename NonlinearRegression<T>::Points& points, 
              const typename NonlinearRegression<T>::FirstPartials& partials,
              const typename NonlinearRegression<T>::Parameters& parameters)
const
{
    DenseMatrix<T> Z(points.size(), partials.size());
    for(size_t j = 0; j < Z.getNumCols(); ++j)
    {
        for(size_t i = 0; i < Z.getNumRows(); ++i)
        {
            Z[i][j] = partials[j](parameters, points[i].x());
        }
    }
    return Z;
}

template<class T>
MyVector<T>
NonlinearRegression<T>::
createDVector(const typename NonlinearRegression<T>::Points& points,
              const typename NonlinearRegression<T>::Function& f,
              const typename NonlinearRegression<T>::Parameters& parameters) 
const
{
    MyVector<T> D(points.size());
    for(size_t i = 0; i < D.getSize(); ++i)
    {
        D[i] = points[i].y() - f(parameters, points[i].x());
    }
    return D;
}

